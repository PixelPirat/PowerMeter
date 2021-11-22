#include "powermeterAnalyzer.h"
#include "powermeterAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

powermeterAnalyzer::powermeterAnalyzer()
:	Analyzer2(),  
	mSettings( new powermeterAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

powermeterAnalyzer::~powermeterAnalyzer()
{
	KillThread();
}

void powermeterAnalyzer::SetupResults()
{
	mResults.reset( new powermeterAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void powermeterAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );

	if( mSerial->GetBitState() == BIT_HIGH )
	{
		mSerial->AdvanceToNextEdge();
	}

	U32 sample_start_bit = 19000;	// 10sample/µs => 2ms = 20000sample

	for( ; ; )
	{
		U8 data = 0;
		U8 mask = 1 << 7;
		
		mSerial->AdvanceToNextEdge(); //rising edge -- beginning of the start bit

		// test for start
		U32 state_changes = mSerial->Advance( sample_start_bit );

		if(state_changes != 0)
		{
			// no start, skip 100ms
			mSerial->Advance( (U32)(1000000) );

			continue;
		}
		else
		{
			if(mSerial->GetBitState() == BIT_HIGH)
			{
				mSerial->AdvanceToNextEdge();
			}
			else
			{
				// error
				continue;
			}

			U64 starting_sample = mSerial->GetSampleNumber();
			for(U32 i = 1; i <= 72; i++)
			{
				U32 sample_bitchanges = mSerial->Advance( (U32)(2000) );	// 2000 = 200µs

				//let's put a dot exactly where we sample this bit:
				mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

				if( mSerial->GetBitState() == BIT_LOW )
				{
					data |= mask;
					mSerial->AdvanceToNextEdge();
				}

				mSerial->AdvanceToNextEdge();

				mask = mask >> 1;

				if( !(i % 8) )
				{
					//we have a byte to save.
					Frame frame;
					frame.mData1 = data;
					frame.mFlags = 0;
					frame.mStartingSampleInclusive = starting_sample;
					frame.mEndingSampleInclusive = mSerial->GetSampleNumber();

					mResults->AddFrame( frame );
					mResults->CommitResults();
					ReportProgress( frame.mEndingSampleInclusive );

					data = 0;
					mask = 1 << 7;

					starting_sample = mSerial->GetSampleNumber();
				}
			}
		}
	}
}

bool powermeterAnalyzer::NeedsRerun()
{
	return false;
}

U32 powermeterAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 powermeterAnalyzer::GetMinimumSampleRateHz()
{
	return mSettings->mBitRate * 4;
}

const char* powermeterAnalyzer::GetAnalyzerName() const
{
	return "Power Meter SWP";
}

const char* GetAnalyzerName()
{
	return "Power Meter SWP";
}

Analyzer* CreateAnalyzer()
{
	return new powermeterAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}