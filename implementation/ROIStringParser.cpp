ROIStringParser::ROIStringParser(const string newString, int newNumberOfROIlines, int newNumberOfRuns, int newNumberOfPeriods):
	sourceString(newString), numberOfROIlines(newNumberOfROIlines), numberOfPeriods(newNumberOfPeriods) {

	string dummy, allRunStr, allPeriodStr, runStr, periodStr, vBoundsStr, dVdtBoundsStr, weightStr;
	int run, period;

	istringstream ss(sourceString);

	for (int i = 0; i < numberOfROIlines; i++) {
		getline(ss, dummy, '['); 
		getline(ss, allRunStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, allPeriodStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, vBoundsStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, dVdtBoundsStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, weightStr, ']');

		istringstream ss2(allRunStr);
		istringstream ss3(periodStr);
		while (getline(ss2,runStr,',')) {
			getline(ss3, periodStr, ',');
			run = toInt(runStr);
			period = toInt(periodStr);

			ROIs[run*numberOfRuns+period].push_back();			
		}		
		

		istringstream ss2(vBoundsStr);
		istringstream ss2(dVdtBoundsStr);
		istringstream ss2(weightStr);
	}


}

string ROIStringParser::toString() const {
	return sourceString;
}

int ROIStringParser::getNumberOfROIlines() const {
	
}
        int getNumberOfPeriods() const;
        int getNumberOfRuns() const;
        int getNumberOfROIsForRunAndPeriod(int nRun, int nPeriod) const;


