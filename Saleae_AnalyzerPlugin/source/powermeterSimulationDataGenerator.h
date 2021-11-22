#ifndef POWERMETER_SIMULATION_DATA_GENERATOR
#define POWERMETER_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class powermeterAnalyzerSettings;

class powermeterSimulationDataGenerator
{
public:
	powermeterSimulationDataGenerator();
	~powermeterSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, powermeterAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	powermeterAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //POWERMETER_SIMULATION_DATA_GENERATOR