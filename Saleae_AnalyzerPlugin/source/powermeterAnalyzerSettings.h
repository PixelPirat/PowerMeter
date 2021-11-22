#ifndef POWERMETER_ANALYZER_SETTINGS
#define POWERMETER_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class powermeterAnalyzerSettings : public AnalyzerSettings
{
public:
	powermeterAnalyzerSettings();
	virtual ~powermeterAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mBitRate;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //POWERMETER_ANALYZER_SETTINGS
