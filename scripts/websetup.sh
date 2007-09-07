#!/bin/sh

eval `./proccgi.sh $* `


echo Content-type: text/xml
echo ""

echo "<?xml version=\"1.0\"?>"


/bin/cat <<EOF
<TestProgram>
	<Dimensions>${FORM_Dimensions}</Dimensions>
	<VerboseLevel>${FORM_VerboseLevel}</VerboseLevel>
	<Seed>${FORM_Seed}</Seed>
	 
	<SamplingFrequency>${FORM_SamplingFrequency}</SamplingFrequency> 
	<StartingPoints>${FORM_StartingPoints}</StartingPoints> 
        <Bounds> <!-- LowerBound UpperBound --> 
                        ${FORM_Bounds}
        </Bounds> 
        <WorkingDirectory>${FORM_WorkingDirectory}</WorkingDirectory> 
        <PrintParameterFile>${FORM_PrintParameterFile}</PrintParameterFile> 
        <FitterType>${FORM_FitterType}</FitterType> 
        <FitterParameters> 
          <NumberOfPoints>${FORM_FitterParameters_NumberOfPoints}</NumberOfPoints> 
        </FitterParameters> 
 
        <TracesReaderType>${FORM_TracesReaderType}</TracesReaderType> 
        
        <TracesReaderParameters> 
	  <NumberOfRuns>${FORM_TracesReaderParameters_NumberOfRuns}</NumberOfRuns> 
	  <NumberOfRunParameters>${FORM_TracesReaderParameters_NumberOfRunParameters}</NumberOfRunParameters> 
	  <RunParameters> 
		${FORM_TracesReaderParameters_RunParameters}
	  </RunParameters> 
	  <NumberOfPeriods>${FORM_TracesReaderParameters_NumberOfPeriods}</NumberOfPeriods> 
	  <Periods> 
	    <!-- PeriodStart PeriodSabs Weight --> 
		    ${FORM_TracesReaderParameters_Periods}
	  </Periods> 
	 
          <NumberOfRecordSites>${FORM_TracesReaderParameters_NumberOfRecordSites}</NumberOfRecordSites> 
	  <RecordSites> 
	        <!-- Weights --> 
		${FORM_TracesReaderParameters_RecordSites} 
	  </RecordSites> 
	  <OutputFilePrefix>${FORM_TracesReaderParameters_OutputFilePrefix}</OutputFilePrefix> 
        </TracesReaderParameters> 

        <ModelType>${FORM_ModelType}</ModelType> 
        <ModelParameters> 
	   <GenesisLocation>${FORM_ModelParameters_GenesisLocation}</GenesisLocation> 
 
	   <ModelDirectory>${FORM_ModelParameters_ModelDirectory}</ModelDirectory> 
	   <OutputDirectory>${FORM_ModelParameters_OutputDirectory}</OutputDirectory> 
	   <ModelSource>${FORM_ModelParameters_ModelSource}</ModelSource> 
	   <ParameterFile>${FORM_ModelParameters_ParameterFile}</ParameterFile> 
	 
	   <ShowExecuteOutput>${FORM_ModelParameters_ShowExecuteOutput}</ShowExecuteOutput>  
        </ModelParameters>  	
	 
        <ExperimentType>${FORM_ExperimentType}</ExperimentType> 
        <ExperimentParameters> 
	   <Parameters>${FORM_ExperimentParameters}</Parameters>	 
        </ExperimentParameters> 
	 
        <FitnessCalculatorType>Matrix</FitnessCalculatorType> 
        <FitnessCalculatorParameters> 
  	  <VdVdtMatrixType>Direct</VdVdtMatrixType> 
	  <VdVdtMatrixParameters> 
	    <vLength>${FORM_VdVdtMatrixParameters_vLength}</vLength> 
	    <dVdtLength>${FORM_VdVdtMatrixParameters_dVdtLength}</dVdtLength> 
	    <minimalV>${FORM_VdVdtMatrixParameters_minimalV}</minimalV> 
	    <maximalV>0.05</maximalV> 
                   <comparePrecision>1e-15</comparePrecision> 
	    <numericOutputFormat>0</numericOutputFormat> 
	    <SumOfSquareRoots>1</SumOfSquareRoots> 
	</VdVdtMatrixParameters> 
	<enableFileExport>${FORM_FitnessCalculatorParameters_enableFileExport}</enableFileExport> 
	<exportFile>${FORM_FitnessCalculatorParameters_exportFile}</exportFile> 
      </FitnessCalculatorParameters> 
EOF

case "${FORM_FitterType}" in
	Random) /bin/cat <<EOF
	    <FitterType>Random</FitterType>
	    <FitterParameters> 
	        <NumberOfPoints>${FORM_Random_NumberOfPoints}</NumberOfPoints> 
	    </FitterParameters>
EOF
	break ;;

	Swarm) /bin/cat <<EOF
	    <FitterType>Swarm</FitterType> 
	    <FitterParameters> 
	        <NumberOfPoints>${FORM_Swarm_NumberOfRuns}</NumberOfPoints> 
	    </FitterParameters>
EOF
	break ;;

	NOMAD) /bin/cat <<EOF
	    <FitterType>NOMAD</FitterType> 
	    <FitterParameters> 
	       <OutputFile>${FORM_NOMAD_OutputFile}</OutputFile> 
	    <!-- parameters.dat --> 
	      <COARSENING_EXPONENT>   ${FORM_NOMAD_COARSENING_EXPONENT}</COARSENING_EXPONENT> <!-- Exponent to enlarge poll size in case of failure --> 
	      <INITIAL_POLL_SIZE>     ${FORM_NOMAD_INITIAL_POLL_SIZE}</INITIAL_POLL_SIZE> 
	      <POLL_BASIS>            ${FORM_NOMAD_POLL_BASIS}</POLL_BASIS> 
	      <REFINING_EXPONENT>     ${FORM_NOMAD_REFINING_EXPONENT}</REFINING_EXPONENT> 
	      <MAX_POLL_SIZE>         ${FORM_NOMAD_MAX_POLL_SIZE}</MAX_POLL_SIZE> 
	      
	      <POLL_ORDER>            ${FORM_NOMAD_POLL_ORDER}</POLL_ORDER> <!-- Dynamic 0/1 --> 
	      <POLL_COMPLETE>         ${FORM_NOMAD_POLL_COMPLETE}</POLL_COMPLETE> <!-- Opportunistic 0/1 --> 
	      <POLL_DIRECTIONS>       ${FORM_NOMAD_POLL_DIRECTIONS}</POLL_DIRECTIONS> <!-- MADS directions 0/1/2/3/4--> 
	      
	      <INITIAL_SEARCH>        ${FORM_NOMAD_INITIAL_SEARCH}</INITIAL_SEARCH> <!-- No initial search 0/1/3 --> 
	      <INITIAL_COMPLETE>      ${FORM_NOMAD_INITIAL_COMPLETE}</INITIAL_COMPLETE> <!-- Opportunistic 0/1 --> 
	      <INITIAL_POINTS>        ${FORM_NOMAD_INITIAL_POINTS}</INITIAL_POINTS> <!-- Number of points after initial search --> 
	      
	      <ITERATIVE_SEARCH>      ${FORM_NOMAD_ITERATIVE_SEARCH}</ITERATIVE_SEARCH> <!-- Random search at start every iteration 0/1/2 --> 
	      <ITERATIVE_COMPLETE>    ${FORM_NOMAD_ITERATIVE_COMPLETE}</ITERATIVE_COMPLETE> <!-- Opportunistic 0/1 --> 
	      <ITERATIVE_POINTS>      ${FORM_NOMAD_ITERATIVE_POINTS}</ITERATIVE_POINTS> <!-- Number of points generated by iteration --> 
	      
              <SPECULATIVE_SEARCH>    ${FORM_NOMAD_SPECULATIVE_SEARCH}</SPECULATIVE_SEARCH> <!-- Do a search in poll direction (only MADS) 0/1 --> 
	      
	      <POLL_SIZE_TERM>        ${FORM_NOMAD_POLL_SIZE_TERM}</POLL_SIZE_TERM> <!-- Terminate when poll size is smaller than this value --> 
	      <ITERATIONS>            ${FORM_NOMAD_ITERATIONS}</ITERATIONS> <!-- Max number of iterations --> 
	      <TRUTH_EVALS>           ${FORM_NOMAD_TRUTH_EVALS}</TRUTH_EVALS> <!-- Max number of truth evaluations --> 
	      <NEW_TRUTH_EVALS>       ${FORM_NOMAD_NEW_TRUTH_EVALS}</NEW_TRUTH_EVALS> <!-- Max number of non-cache truth evaluations --> 
	      <CONSECUTIVE_FAILS>     ${FORM_NOMAD_CONSECUTIVE_FAILS}</CONSECUTIVE_FAILS> <!-- Max number of consecutive better-solution-failures --> 
	      <FILTER_RELATIVE>       ${FORM_NOMAD_FILTER_RELATIVE}</FILTER_RELATIVE> <!-- HMax is relative to starting point 0/1 --> 
	      <HMAX>                  ${FORM_NOMAD_HMAX}</HMAX> 
	      <HMIN>                  ${FORM_NOMAD_HMIN}</HMIN>      
	      <FILTER_NORM>           ${FORM_NOMAD_FILTER_NORM}</FILTER_NORM> <!-- L-2 squared norm 0/1/2 --> 
	      
	      <SURROGATE_TOLERANCE>   ${FORM_NOMAD_SURROGATE_TOLERANCE}</SURROGATE_TOLERANCE> 
	      
	      <!-- description.dat --> 
	      <SCALING_CHOICE>        ${FORM_NOMAD_SCALING_CHOICE}</SCALING_CHOICE> <!-- No scaling 0/1/2/3 --> 
	      <GEN_CONS_NB>           ${FORM_NOMAD_GEN_CONS_NB}</GEN_CONS_NB> <!-- Number of constraints --> 
	      <USE_SURROGATE>         ${FORM_NOMAD_USE_SURROGATE}</USE_SURROGATE> <!-- Use surrogate 0/1 --> 
	      
	      <START_PT_FILE>         ${FORM_NOMAD_START_PT_FILE}</START_PT_FILE> 
	      <BOUNDS_FILE>           ${FORM_NOMAD_BOUNDS_FILE}</BOUNDS_FILE> 
	      <RESULTS_FILE>          ${FORM_NOMAD_RESULTS_FILE}</RESULTS_FILE> 
	    </FitterParameters>
EOF
	break ;;

	EO) /bin/cat <<EOF
	    <FitterType>EO</FitterType> 
	    <FitterParameters> 
	      <popSize>      ${FORM_EO_popSize}</popSize> 
	      <nbOffspring>  ${FORM_EO_nbOffspring}</nbOffspring> 
	      <replacement>  ${FORM_EO_replacement}</replacement> 
	      <maxGen>       ${FORM_EO_maxGen}</maxGen> 
	      <minGen>       ${FORM_EO_minGen}</minGen> 
	      <maxEval>      ${FORM_EO_maxEval}</maxEval> 
	      <targetFitness>${FORM_EO_targetFitness}</targetFitness> 
	      <steadyGen>    ${FORM_EO_steadyGen}</steadyGen> 
	      <crossType>    ${FORM_EO_crossType}</crossType> 
	      <crossObj>     ${FORM_EO_crossObj}</crossObj>  
	      <TauLoc>       ${FORM_EO_TauLoc}</TauLoc> 
	      <TauGlob>      ${FORM_EO_TauGlob}</TauGlob> 
	      <Beta>         ${FORM_EO_Beta}</Beta> 
	    </FitterParameters> 
EOF
	break ;;

	*) break ;;
esac

echo "</TestProgram>"
