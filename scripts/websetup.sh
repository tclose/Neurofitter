#!/bin/sh

echo Content-type: text/xml
echo ""

/bin/cat <<EOF
	<TestProgram>
		<Dimensions>${Dimensions}</Dimensions>
		<VerboseLevel>${VerboseLevel}</VerboseLevel>
		<Seed>${Seed}</Seed>
	 
		<SamplingFrequency>${SamplingFrequency}</SamplingFrequency> 
		<StartingPoints>${StartingPoints}</StartingPoints> 
	        <Bounds> <!-- LowerBound UpperBound --> 
                         ${Bounds}
	        </Bounds> 
	        <WorkingDirectory>${WorkingDirectory}</WorkingDirectory> 
	        <PrintParameterFile>${PrintParameterFile}</PrintParameterFile> 
	        <FitterType>${FitterType}</FitterType> 
	        <FitterParameters> 
	          <NumberOfPoints>${FitterParameters_NumberOfPoints}</NumberOfPoints> 
	        </FitterParameters> 
	 
	        <TracesReaderType>${TracesReaderType}</TracesReaderType> 
	        
                <TracesReaderParameters> 
		  <NumberOfRuns>${TracesReaderParameters_NumberOfRuns}</NumberOfRuns> 
		  <NumberOfRunParameters>${TracesReaderParameters_NumberOfRunParameters}</NumberOfRunParameters> 
		  <RunParameters> 
			${TracesReaderParameters_RunParameters}
		  </RunParameters> 
		  <NumberOfPeriods>${TracesReaderParameters_NumberOfPeriods}</NumberOfPeriods> 
		  <Periods> 
		    <!-- PeriodStart PeriodSabs Weight --> 
		    ${TracesReaderParameters_Periods}
		  </Periods> 
	 
	          <NumberOfRecordSites>${TracesReaderParameters_NumberOfRecordSites}</NumberOfRecordSites> 
		  <RecordSites> 
		        <!-- Weights --> 
			${TracesReaderParameters_RecordSites} 
		  </RecordSites> 
		  <OutputFilePrefix>${TracesReaderParameters_OutputFilePrefix}</OutputFilePrefix> 
	        </TracesReaderParameters> 

	        <ModelType>${ModelType}</ModelType> 
	        <ModelParameters> 
		   <GenesisLocation>${ModelParameters_GenesisLocation}</GenesisLocation> 
	 
		   <ModelDirectory>${ModelParameters_ModelDirectory}</ModelDirectory> 
		   <OutputDirectory>${ModelParameters_OutputDirectory}</OutputDirectory> 
		   <ModelSource>${ModelParameters_ModelSource}</ModelSource> 
		   <ParameterFile>${ModelParameters_ParameterFile}</ParameterFile> 
	 
		   <ShowExecuteOutput>${ModelParameters_ShowExecuteOutput}</ShowExecuteOutput>  
	        </ModelParameters>  	
	 
	        <ExperimentType>${ExperimentType}</ExperimentType> 
	        <ExperimentParameters> 
		   <Parameters>${ExperimentParameters}</Parameters>	 
	        </ExperimentParameters> 
	 
	        <FitnessCalculatorType>Matrix</FitnessCalculatorType> 
	        <FitnessCalculatorParameters> 
	  	  <VdVdtMatrixType>Direct</VdVdtMatrixType> 
		  <VdVdtMatrixParameters> 
		    <vLength>${VdVdtMatrixParameters_vLength}</vLength> 
		    <dVdtLength>${VdVdtMatrixParameters_dVdtLength}</dVdtLength> 
		    <minimalV>${VdVdtMatrixParameters_minimalV}</minimalV> 
		    <maximalV>0.05</maximalV> 
		    <comparePrecision>1e-15</comparePrecision> 
		    <numericOutputFormat>0</numericOutputFormat> 
		    <SumOfSquareRoots>1</SumOfSquareRoots> 
		</VdVdtMatrixParameters> 
		<enableFileExport>${FitnessCalculatorParameters_enableFileExport}</enableFileExport> 
		<exportFile>${FitnessCalculatorParameters_exportFile}</exportFile> 
	      </FitnessCalculatorParameters> 
EOF

case "${FitterType}" in
	Random) /bin/cat <<EOF
	    <FitterType>Random</FitterType>
	    <FitterParameters> 
	        <NumberOfPoints>${Random_NumberOfPoints}</NumberOfPoints> 
	    </FitterParameters>
EOF
            ; break ;;

	Swarm) /bin/cat <<EOF
	    <FitterType>Swarm</FitterType> 
	    <FitterParameters> 
	        <NumberOfPoints>${Swarm_NumberOfRuns}</NumberOfPoints> 
	    </FitterParameters>
EOF
	    ; break ;;

	NOMAD) /bin/cat <<EOF
	    <FitterType>NOMAD</FitterType> 
	    <FitterParameters> 
	       <OutputFile>${NOMAD_OutputFile}</OutputFile> 
	    <!-- parameters.dat --> 
	      <COARSENING_EXPONENT>   ${NOMAD_COARSENING_EXPONENT}</COARSENING_EXPONENT> <!-- Exponent to enlarge poll size in case of failure --> 
	      <INITIAL_POLL_SIZE>     ${NOMAD_INITIAL_POLL_SIZE}</INITIAL_POLL_SIZE> 
	      <POLL_BASIS>            ${NOMAD_POLL_BASIS}</POLL_BASIS> 
	      <REFINING_EXPONENT>     ${NOMAD_REFINING_EXPONENT}</REFINING_EXPONENT> 
	      <MAX_POLL_SIZE>         ${NOMAD_MAX_POLL_SIZE}</MAX_POLL_SIZE> 
	      
	      <POLL_ORDER>            ${NOMAD_POLL_ORDER}</POLL_ORDER> <!-- Dynamic 0/1 --> 
	      <POLL_COMPLETE>         ${NOMAD_POLL_COMPLETE}</POLL_COMPLETE> <!-- Opportunistic 0/1 --> 
	      <POLL_DIRECTIONS>       ${NOMAD_POLL_DIRECTIONS}</POLL_DIRECTIONS> <!-- MADS directions 0/1/2/3/4--> 
	      
	      <INITIAL_SEARCH>        ${NOMAD_INITIAL_SEARCH}</INITIAL_SEARCH> <!-- No initial search 0/1/3 --> 
	      <INITIAL_COMPLETE>      ${NOMAD_INITIAL_COMPLETE}</INITIAL_COMPLETE> <!-- Opportunistic 0/1 --> 
	      <INITIAL_POINTS>        ${NOMAD_INITIAL_POINTS}</INITIAL_POINTS> <!-- Number of points after initial search --> 
	      
	      <ITERATIVE_SEARCH>      ${NOMAD_ITERATIVE_SEARCH}</ITERATIVE_SEARCH> <!-- Random search at start every iteration 0/1/2 --> 
	      <ITERATIVE_COMPLETE>    ${NOMAD_ITERATIVE_COMPLETE}</ITERATIVE_COMPLETE> <!-- Opportunistic 0/1 --> 
	      <ITERATIVE_POINTS>      ${NOMAD_ITERATIVE_POINTS}</ITERATIVE_POINTS> <!-- Number of points generated by iteration --> 
	      
	      <SPECULATIVE_SEARCH>    ${NOMAD_SPECULATIVE_SEARCH}</SPECULATIVE_SEARCH> <!-- Do a search in poll direction (only MADS) 0/1 --> 
	      
	      <POLL_SIZE_TERM>        ${NOMAD_POLL_SIZE_TERM}</POLL_SIZE_TERM> <!-- Terminate when poll size is smaller than this value --> 
	      <ITERATIONS>            ${NOMAD_ITERATIONS}</ITERATIONS> <!-- Max number of iterations --> 
	      <TRUTH_EVALS>           ${NOMAD_TRUTH_EVALS}</TRUTH_EVALS> <!-- Max number of truth evaluations --> 
	      <NEW_TRUTH_EVALS>       ${NOMAD_NEW_TRUTH_EVALS}</NEW_TRUTH_EVALS> <!-- Max number of non-cache truth evaluations --> 
	      <CONSECUTIVE_FAILS>     ${NOMAD_CONSECUTIVE_FAILS}</CONSECUTIVE_FAILS> <!-- Max number of consecutive better-solution-failures --> 
	      <FILTER_RELATIVE>       ${NOMAD_FILTER_RELATIVE}</FILTER_RELATIVE> <!-- HMax is relative to starting point 0/1 --> 
	      <HMAX>                  ${NOMAD_HMAX}</HMAX> 
	      <HMIN>                  ${NOMAD_HMIN}</HMIN>      
	      <FILTER_NORM>           ${NOMAD_FILTER_NORM}</FILTER_NORM> <!-- L-2 squared norm 0/1/2 --> 
	      
	      <SURROGATE_TOLERANCE>   ${NOMAD_SURROGATE_TOLERANCE}</SURROGATE_TOLERANCE> 
	      
	      <!-- description.dat --> 
	      <SCALING_CHOICE>        ${NOMAD_SCALING_CHOICE}</SCALING_CHOICE> <!-- No scaling 0/1/2/3 --> 
	      <GEN_CONS_NB>           ${NOMAD_GEN_CONS_NB}</GEN_CONS_NB> <!-- Number of constraints --> 
	      <USE_SURROGATE>         ${NOMAD_USE_SURROGATE}</USE_SURROGATE> <!-- Use surrogate 0/1 --> 
	      
	      <START_PT_FILE>         ${NOMAD_START_PT_FILE}</START_PT_FILE> 
	      <BOUNDS_FILE>           ${NOMAD_BOUNDS_FILE}</BOUNDS_FILE> 
	      <RESULTS_FILE>          ${NOMAD_RESULTS_FILE}</RESULTS_FILE> 
	    </FitterParameters>
EOF
	    ; break ;;

	EO) /bin/cat <<EOF
	    <FitterType>EO</FitterType> 
	    <FitterParameters> 
	      <popSize>      ${EO_popSize}</popSize> 
	      <nbOffspring>  ${EO_nbOffspring}</nbOffspring> 
	      <replacement>  ${EO_replacement}</replacement> 
	      <maxGen>       ${EO_maxGen}</maxGen> 
	      <minGen>       ${EO_minGen}</minGen> 
	      <maxEval>      ${EO_maxEval}</maxEval> 
	      <targetFitness>${EO_targetFitness}</targetFitness> 
	      <steadyGen>    ${EO_steadyGen}</steadyGen> 
	      <crossType>    ${EO_crossType}</crossType> 
	      <crossObj>     ${EO_crossObj}</crossObj>  
	      <TauLoc>       ${EO_TauLoc}</TauLoc> 
	      <TauGlob>      ${EO_TauGlob}</TauGlob> 
	      <Beta>         ${EO_Beta}</Beta> 
	    </FitterParameters> 
EOF
	    ; break ;;

	*) break ;;
    esac
	    
echo "     </TestProgram> "
