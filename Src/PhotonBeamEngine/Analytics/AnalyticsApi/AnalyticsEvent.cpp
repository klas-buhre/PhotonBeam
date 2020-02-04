#include "AnalyticsEvent.hpp"

using namespace Pht;

AnalyticsEvent::AnalyticsEvent(Kind kind) :
    mKind {kind} {}

BusinessAnalyticsEvent::BusinessAnalyticsEvent(const std::string& currency,
                                               int amount,
                                               const std::string& itemType,
                                               const std::string& itemId,
                                               const std::string& cartType) :
    AnalyticsEvent {AnalyticsEvent::Kind::Business},
    mCurrency {currency},
    mAmount {amount},
    mItemType {itemType},
    mItemId {itemId},
    mCartType {cartType} {}

ResourceAnalyticsEvent::ResourceAnalyticsEvent(ResourceFlow resourceFlow,
                                               const std::string& currency,
                                               float amount,
                                               const std::string& itemType,
                                               const std::string& itemId) :
    AnalyticsEvent {AnalyticsEvent::Kind::Resource},
    mResourceFlow {resourceFlow},
    mCurrency {currency},
    mAmount {amount},
    mItemType {itemType},
    mItemId {itemId} {}

ProgressionAnalyticsEvent::ProgressionAnalyticsEvent(ProgressionStatus progressionStatus,
                                                     const std::string& progression,
                                                     Pht::Optional<int> score) :
    AnalyticsEvent {AnalyticsEvent::Kind::Progression},
    mProgressionStatus {progressionStatus},
    mProgression {progression},
    mScore {score} {}

ErrorAnalyticsEvent::ErrorAnalyticsEvent(ErrorSeverity severity, const std::string& message) :
    AnalyticsEvent {AnalyticsEvent::Kind::Error},
    mSeverity {severity},
    mMessage {message} {}

CustomAnalyticsEvent::CustomAnalyticsEvent(const std::string& id) :
    AnalyticsEvent {AnalyticsEvent::Kind::Custom},
    mId {id} {}
