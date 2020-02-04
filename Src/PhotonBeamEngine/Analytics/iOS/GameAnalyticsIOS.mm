#include "Analytics.hpp"
#include "AnalyticsConfig.hpp"

#import <Foundation/Foundation.h>

#import "GameAnalytics.h"

#include "AnalyticsEvent.hpp"

#define DEBUG

using namespace Pht;

namespace {
    const std::string eventSubmissionEnabledFilename {"event_submission_enabled.dat"};

    GAResourceFlowType ToGAResourceFlowType(ResourceFlow resourceFlow) {
        switch (resourceFlow) {
            case ResourceFlow::Source:
                return GAResourceFlowTypeSource;
            case ResourceFlow::Sink:
                return GAResourceFlowTypeSink;
        }
    }
    
    GAProgressionStatus ToGAProgressionStatus(ProgressionStatus progressionStatus) {
        switch (progressionStatus) {
            case ProgressionStatus::Start:
                return GAProgressionStatusStart;
            case ProgressionStatus::Fail:
                return GAProgressionStatusFail;
            case ProgressionStatus::Complete:
                return GAProgressionStatusComplete;
        }
    }
    
    GAErrorSeverity ToGAErrorSeverity(ErrorSeverity severity) {
        switch (severity) {
            case Pht::ErrorSeverity::Debug:
                return GAErrorSeverityDebug;
            case Pht::ErrorSeverity::Info:
                return GAErrorSeverityInfo;
            case Pht::ErrorSeverity::Warning:
                return GAErrorSeverityWarning;
            case Pht::ErrorSeverity::Error:
                return GAErrorSeverityError;
            case Pht::ErrorSeverity::Critical:
                return GAErrorSeverityCritical;
        }
    }

#ifdef ANALYTICS_ENABLED
    void InitGameAnalyticsIOS() {
#ifdef DEBUG
        [GameAnalytics setEnabledInfoLog:YES];
        [GameAnalytics setEnabledVerboseLog:YES];
#endif
        
        NSString* buildVersion = [NSString stringWithUTF8String:GetBuildVersion().c_str()];
        [GameAnalytics configureBuild:buildVersion];
        [GameAnalytics configureAvailableResourceCurrencies:@[@"coins"]];
        [GameAnalytics configureAvailableResourceItemTypes:@[@"moves", @"lives", @"coins"]];
        
        NSString* analyticsGameKey = [NSString stringWithUTF8String:GetAnalyticsGameKey().c_str()];
        NSString* analyticsGameSecret = [NSString stringWithUTF8String:GetAnalyticsGameSecret().c_str()];
        [GameAnalytics initializeWithGameKey:analyticsGameKey gameSecret:analyticsGameSecret];
    }
#endif

    class GameAnalyticsIOS: public IAnalytics {
    public:
        void InitAnalytics() override {
#ifdef ANALYTICS_ENABLED
            InitGameAnalyticsIOS();
#endif
        }
    
        void AddEvent(const AnalyticsEvent& event) override {
#ifdef ANALYTICS_ENABLED
            switch (event.GetKind()) {
                case AnalyticsEvent::Kind::Business:
                    AddBusinessEvent(static_cast<const BusinessAnalyticsEvent&>(event));
                    break;
                case AnalyticsEvent::Kind::Resource:
                    AddResourceEvent(static_cast<const ResourceAnalyticsEvent&>(event));
                    break;
                case AnalyticsEvent::Kind::Progression:
                    AddProgressionEvent(static_cast<const ProgressionAnalyticsEvent&>(event));
                    break;
                case AnalyticsEvent::Kind::Error:
                    AddErrorEvent(static_cast<const ErrorAnalyticsEvent&>(event));
                    break;
                case AnalyticsEvent::Kind::Custom:
                    AddCustomEvent(static_cast<const CustomAnalyticsEvent&>(event));
                    break;
            }
#endif
        }
        
    private:
        void AddBusinessEvent(const BusinessAnalyticsEvent& event) {
            [GameAnalytics addBusinessEventWithCurrency:[NSString stringWithUTF8String:event.mCurrency.c_str()]
                           amount:event.mAmount
                           itemType:[NSString stringWithUTF8String:event.mItemType.c_str()]
                           itemId:[NSString stringWithUTF8String:event.mItemId.c_str()]
                           cartType:[NSString stringWithUTF8String:event.mCartType.c_str()]
                           receipt:nil];
        }
        
        void AddResourceEvent(const ResourceAnalyticsEvent& event) {
            [GameAnalytics addResourceEventWithFlowType:ToGAResourceFlowType(event.mResourceFlow)
                           currency:[NSString stringWithUTF8String:event.mCurrency.c_str()]
                           amount:@(event.mAmount)
                           itemType:[NSString stringWithUTF8String:event.mItemType.c_str()]
                           itemId:[NSString stringWithUTF8String:event.mItemId.c_str()]];
        }

        void AddProgressionEvent(const ProgressionAnalyticsEvent& event) {
            if (event.mScore.HasValue()) {
                [GameAnalytics addProgressionEventWithProgressionStatus:ToGAProgressionStatus(event.mProgressionStatus)
                               progression01:[NSString stringWithUTF8String:event.mProgression.c_str()]
                               progression02:nil
                               progression03:nil
                               score:event.mScore.GetValue()];
            } else {
                [GameAnalytics addProgressionEventWithProgressionStatus:ToGAProgressionStatus(event.mProgressionStatus)
                               progression01:[NSString stringWithUTF8String:event.mProgression.c_str()]
                               progression02:nil
                               progression03:nil];
            }
        }
        
        void AddErrorEvent(const ErrorAnalyticsEvent& event) {
            [GameAnalytics addErrorEventWithSeverity:ToGAErrorSeverity(event.mSeverity)
                           message:[NSString stringWithUTF8String:event.mMessage.c_str()]];
        }
        
        void AddCustomEvent(const CustomAnalyticsEvent& event) {
            [GameAnalytics addDesignEventWithEventId:[NSString stringWithUTF8String:event.mId.c_str()]];
        }
    };
}

std::unique_ptr<IAnalytics> Pht::CreateAnalyticsApi() {
    return std::make_unique<GameAnalyticsIOS>();
}
