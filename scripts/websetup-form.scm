#!/bin/sh
#|  -*- Hen -*-
exec csi -I "$1" -s "$0" "$@"
|#
;; Evaluation of this file yields an HTML document

;;
;; A list of variables to be included in the main HTML form. 
;; The format is (name  default-value [...] [(label ...) (type ...)] [children ...])
;;
;; The default label is the name of the variable.
;; If the type is not specified, default is text.
;; 
(define form-variables
  `((Dimensions          2     (label "Number of model parameters to be fit: "))
    (VerboseLevel        4     (label "Verbosity level: "))
    (Seed                1550  (label "Random seed for stochastic optimization: "))
    (SamplingFrequency   5000  (label "Sampling frequency of experimental data and model output: "))
    (StartingPoints      1.0 1.0  (label "Parameters to initialize the optimization algorithm: "))
    ;; LowerBound UpperBound
    (Bounds              1.0 10000.0 
			 1.0 50000.0 
			 (label "Bounds of model parameters: ")
			 (type textarea 10 20 2 2))
    (WorkingDirectory    "."    (label "Location where NeuroFitter should write its output: "))
    (PrintParameterFile  1      
			 (label "Write NeuroFitter settings to stdout: ")
			 (type checkbox))
    (TracesReaderType    Normal
			 (label "Traces reader type: ")
			 (type select Normal))
    (TracesReaderParameters
     (NumberOfRuns             1 (label "Number of times the model has to be run for every fitness evaluation: "))
     (NumberOfRunParameters    1 (label "Number of parameters for every run: "))
     (RunParameters            0.0 1.0 (label "Run parameters: "))
     (NumberOfPeriods          3 (label "Number of periods: "))
     (Periods      
      ;; PeriodStart PeriodSabs Weight 
      2.0 3.9 0.33 
      1.0 1.5 0.33 
      0.5 0.75 0.33 
      (label "Start and stop times of the periods (one line per period, third number is the weight): ") 
      (type textarea))
     (NumberOfRecordSites      1 (label "Number of recording sites in the cell: "))
     (RecordSites              1 (label "The weight of every recording site: "))
     (OutputFilePrefix         "output" (label "Prefix for files with the experimental and model output data: ")))
    (ModelType                 Genesis
			       (label "Model type: ") 
			       (type select Genesis Neuron))
    (ModelParameters
      (NeuronLocation           "special" (label "Location of Neuron special file: "))
      (GenesisLocation          "genesis" (label "Location of Genesis executable: "))
      (ModelDirectory           "." (label "Directory with model files: "))
      (OutputDirectory          "./outputs" (label "Directory that will contain simulation output files: "))
      (ModelSource              "./model.g" (label "The source model file: "))
      (ParameterFile            "./param.dat" (label "The name of the parameters file that NeuroFitter should write out: "))
      (ShowExecuteOutput        0 (label "Show output of simulation execution: ") (type checkbox)))
    (ExperimentType            Fake
			       (label "NeuroFitter experiment type: ") 
			       (type select Fake File))
    (ExperimentParameters      
     (FilesDirectory  "" (label "Directory with experimental data: "))
     (Parameters      2800.0 11200.0 (label "Parameters for \"fake\" experimental data: ")))
    (FitnessCalculatorType     Matrix 
			       (label "Fitness calculator type: ")
			       (type select Matrix))
    (FitnessCalculatorParameters
     (VdVdtMatrixType   Direct 
			(label "VdVt matrix type: ")
			(type select Direct))
     (VdVdtMatrixParameters
      (vLength     100   (label "The number of bins in the V direction of the VdVdtMatrix: "))
      (dVdtLength  100   (label "The number of bins in the dVdt direction of the VdVdtMatrix: "))
      (minimalV    -0.1  (label "The minimal V in the VdVdtMatrix: "))
      (maximalV    0.05  (label "The maximal V in the VdVdtMatrix: "))
      (comparePrecision     1e-15 (label "Value used to compare if two values are identical (should be set very low: "))
      (numericOutputFormat  0  (label "Show numerical output of the matrices (if verbose level is high): ") (type checkbox))
      (SumOfSquareRoots     0 (label "Calculate the sum of the square roots instead of sum squares when calculating the fitness value: ") 
			    (type checkbox))
     (enableFileExport      1 (label "Export all evaluated parameter sets: ") 
			    (type checkbox))
     (exportFile            "parallelFitness.dat"
			    (label "Name of file to write exported parameter sets to: "))))))


(define Content
 `(html:begin
   
   (Header
    (style "wforms.css")
    (title "NeuroFitter Setup")
    (Date-Revision-yyyymmdd "20070906")
    (Date-Creation-yyyymmdd "20070906")
    (keywords "modeling, neuroscience, NeuroFitter")
;    (Embedded)
    (AuthorAddress "raikov@oist.jp")
    (Revision "1.0"))


  (body 

   (page-content 

    (Section 2 "NeuroFitter Parameters")
    
    (form (@ ((action "websetup.sh") (method "post")))
	  ,(map (lambda (x) `(formvar . ,x)) form-variables)
	  
	  (div (@ (class actions))
	       (input (@ (type submit) (class primaryAction) 
			 (id submit) (value "Generate")))))

    (footer) ))))




(include "websetup-template.scm")

(generate-HTML Content)

