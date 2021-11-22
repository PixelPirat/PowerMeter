#ifndef POWERMETER_ANALYZER_H
#define POWERMETER_ANALYZER_H

#include <Analyzer.h>
#include "powermeterAnalyzerResults.h"
#include "powermeterSimulationDataGenerator.h"

class powermeterAnalyzerSettings;
class ANALYZER_EXPORT powermeterAnalyzer : public Analyzer2
{
public:
	powermeterAnalyzer();
	virtual ~powermeterAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< powermeterAnalyzerSettings > mSettings;
	std::auto_ptr< powermeterAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	powermeterSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //POWERMETER_ANALYZER_H
