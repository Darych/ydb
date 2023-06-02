#include "services.h"

#include <library/cpp/actors/core/actorsystem.h>
#include <library/cpp/actors/core/hfunc.h>
#include <library/cpp/actors/core/interconnect.h>
#include <library/cpp/actors/core/actor_bootstrapped.h>
#include <util/generic/set.h>
#include <util/generic/vector.h>

class TExamplePublishReplicaActor : public TActorBootstrapped<TExamplePublishReplicaActor> {
    const TActorId Owner;
    const TActorId Replica;
    const TString Key;
    const TString Payload;

    void PassAway() override {
        const ui32 replicaNode = Replica.NodeId();
        if (replicaNode != SelfId().NodeId()) {
            const TActorId &interconnectProxy = TlsActivationContext->ExecutorThread.ActorSystem->InterconnectProxy(Replica.NodeId());
            Send(interconnectProxy, new TEvents::TEvUnsubscribe());
        }
        return IActor::PassAway();
    }

    void SomeSleep() {
        Become(&TThis::StateSleep, TDuration::MilliSeconds(250), new TEvents::TEvWakeup());
    }
public:
    static constexpr IActor::EActivityType ActorActivityType() {
        // define app-specific activity tag to track elapsed cpu | handled events | actor count in Solomon
        return EActorActivity::ACTORLIB_COMMON;
    }

    TExamplePublishReplicaActor(TActorId owner, TActorId replica, const TString &key, const TString &payload)
        : Owner(owner)
        , Replica(replica)
        , Key(key)
        , Payload(payload)
    {}

    void Bootstrap() {
        const ui32 flags = IEventHandle::FlagTrackDelivery | IEventHandle::FlagSubscribeOnSession;
        Send(Replica, new TEvExample::TEvReplicaPublish(Key, Payload), flags);
        Become(&TThis::StatePublish);
    }

    STFUNC(StatePublish) {
        Y_UNUSED(ctx);
        switch (ev->GetTypeRewrite()) {
            cFunc(TEvents::TEvPoison::EventType, PassAway);
            cFunc(TEvents::TEvUndelivered::EventType, SomeSleep);
            cFunc(TEvInterconnect::TEvNodeDisconnected::EventType, SomeSleep);
        default:
            break;
        }
    }

    STFUNC(StateSleep) {
        Y_UNUSED(ctx);
        switch (ev->GetTypeRewrite()) {
            cFunc(TEvents::TEvPoison::EventType, PassAway);
            cFunc(TEvents::TEvWakeup::EventType, Bootstrap);
        default:
            break;
        }
    }
};

class TExamplePublishActor : public TActorBootstrapped<TExamplePublishActor> {
    TIntrusiveConstPtr<TExampleStorageConfig> Config;
    const TString Key;
    const TString Payload;
    TVector<TActorId> PublishActors;

    void PassAway() override {
        for (const auto &x : PublishActors)
            Send(x, new TEvents::TEvPoison());
        return IActor::PassAway();
    }
public:
    static constexpr IActor::EActivityType ActorActivityType() {
        // define app-specific activity tag to track elapsed cpu | handled events | actor count in Solomon
        return EActorActivity::ACTORLIB_COMMON;
    }

    TExamplePublishActor(TExampleStorageConfig *config, const TString &key, const TString &what)
        : Config(config)
        , Key(key)
        , Payload(what)
    {}

    void Bootstrap() {
        for (auto &replica : Config->Replicas) {
            const TActorId x = Register(new TExamplePublishReplicaActor(SelfId(), replica, Key, Payload));
            PublishActors.emplace_back(x);
        }

        Become(&TThis::StateWork);
    }

    STFUNC(StateWork) {
        Y_UNUSED(ctx);
        switch (ev->GetTypeRewrite()) {
            cFunc(TEvents::TEvPoison::EventType, PassAway);
        default:
            break;
        }
    }
};

IActor* CreatePublishActor(TExampleStorageConfig *config, const TString &key, const TString &what) {
    return new TExamplePublishActor(config, key, what);
}
