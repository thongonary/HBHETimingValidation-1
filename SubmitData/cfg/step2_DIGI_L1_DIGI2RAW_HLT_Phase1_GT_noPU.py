# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:phase1_2017_hcaldev -s DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@fake --datatier GEN-SIM-DIGI-RAW -n 10 --geometry Extended2017dev --era Run2_2017_HCALdev --eventcontent FEVTDEBUGHLT -n 100 --nThreads 4 --customise_commands if hasattr(process,'generator'): process.generator.PGunParameters.MinE=process.generator.PGunParameters.MaxE=cms.double(1000) --filein file:step1.root --fileout file:step2.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HLT',eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring(
        'file:step1.root'
    ),

    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(10485760),
    fileName = cms.untracked.string('file:step2_pi500_2017_realistic.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.mix.digitizers = cms.PSet(process.theDigitizersValid)

## MARIA temporarily switch off the timeSlew and set the timePhase to the 0.5
##process.mix.digitizers.hcal.doTimeSlew = cms.bool(False)
##process.mix.digitizers.hcal.he.timePhase = cms.double(7.5)
##process.mix.digitizers.hcal.hb.timePhase = cms.double(7.5)

## MARIA temporarily switch off the noise
#process.mix.digitizers.hcal.doNoise = cms.bool(False)

## MARIA temporarily switch off the geantTime
#process.mix.digitizers.hcal.ignoreGeantTime = cms.bool(True)

##  MARIA temporarily switch off the photon statistics
##process.mix.digitizers.hcal.he.doPhotoStatistics = cms.bool(False)


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
##process.GlobalTag = GlobalTag(process.GlobalTag, '81X_upgrade2017_realistic_v25', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.FEVTDEBUGHLToutput_step])

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(8)
process.options.numberOfStreams=cms.untracked.uint32(0)

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
#from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforFullSim

#call to customisation function customizeHLTforFullSim imported from HLTrigger.Configuration.customizeHLTforMC
#process = customizeHLTforFullSim(process)

##from SLHCUpgradeSimulations.Configuration.HCalCustoms import load_HcalHardcode
##process = load_HcalHardcode(process)
##process.es_hardcode.useHEUpgrade = cms.bool(True)
##process.es_hardcode.useHFUpgrade = cms.bool(True)
##process.es_hardcode.heUpgrade.darkCurrent = cms.double(0)
##process.es_hardcode.SiPMCharacteristics[2].crosstalk = cms.double(0.0)
##process.es_hardcode.SiPMCharacteristics[3].crosstalk = cms.double(0.0)
##process.es_hardcode.toGet = cms.untracked.vstring('GainWidths','SiPMParameters','SiPMCharacteristics')

# End of customisation functions

# Customisation from command line
# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)


dumpFile  = open("DumpRECO_Phase1_step2_GT.py", "w")
dumpFile.write(process.dumpPython())
dumpFile.close()
