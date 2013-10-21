function makeChannels
    int i
	float x, dx, minfV, tauN, tauM, tauH

	echo making and saving channel library....
	
	pushe /library

	float NaFThetaM = -34e-3
	float NaFSigmaM = -5e-3	

	//float NaFTauM = 0.5e-3
	
	float NaFThetaN = -29e-3
	float NaFSigmaN	= -4e-3	

	float NaFTauN = 10e-3

	float NaPThetaM = -40e-3
	float NaPSigmaM = -6e-3	
	float NaPThetaH = -48e-3
	float NaPSigmaH = 6e-3	

	float NaPTauH = 10000e-3


    if (!{exists NaF})
	    create tabchannel NaF
	    setfield NaF Ek {ENa} Gbar {GNaF} Ik 0 Gk 0 Xpower 3 Ypower 1 Zpower 0

	    call NaF TABCREATE X {tab_xdivs} {tab_xmin} {tab_xmax}
	    x = {tab_xmin}
	    dx = ({tab_xmax} - {tab_xmin})/{tab_xdivs}
		
		/////
		// The activation of NaF
		/////
		setfield NaF instant {INSTANTX} 
	    for (i = 0; i <= ({tab_xdivs}); i = i + 1)
		    setfield NaF X_A->table[{i}] 1

			minfV = 1 + {exp {(x-NaFThetaM)/NaFSigmaM}}
		    setfield NaF X_B->table[{i}] {minfV}
		    x = x + dx
	    end
	    setfield NaF X_A->calc_mode 1 X_B->calc_mode 1
	    call NaF TABFILL X {tab_xfills + 1} 0
		
		/*
	    for (i = 0; i <= ({tab_xdivs}); i = i + 1)
		    tauM = 2*NaFTauM/{{exp {(x-NaFThetaM)/(2*NaFSigmaM)}} + {exp {-(x-NaFThetaM)/(2*NaFSigmaM)}}}
		    setfield NaF X_A->table[{i}] {tauM}

			minfV = 1/{1 + {exp {(x-NaFThetaM)/NaFSigmaM}}}
		    setfield NaF X_B->table[{i}] {minfV}
		    x = x + dx
	    end
	    tweaktau NaF X
	    setfield NaF X_A->calc_mode 0 X_B->calc_mode 0
	    call NaF TABFILL X {tab_xfills + 1} 0
		*/

		/////
		// The inactivation of NaF
		///// 
	    call NaF TABCREATE Y {tab_xdivs} {tab_xmin}	{tab_xmax}
	    x = {tab_xmin}
	    dx = ({tab_xmax} - {tab_xmin})/{tab_xdivs}

	    for (i = 0; i <= ({tab_xdivs}); i = i + 1)
		    tauN = 2*NaFTauN/{{exp {(x-NaFThetaN)/(2*NaFSigmaN)}} + {exp {-(x-NaFThetaN)/(2*NaFSigmaN)}}}
			setfield NaF Y_A->table[{i}] {tauN}
			minfV = 1/{1 + {exp {-(x-NaFThetaN)/NaFSigmaN}}}
		    setfield NaF Y_B->table[{i}] {minfV}
		    x = x + dx
	    end
	    tweaktau NaF Y
	    setfield NaF Y_A->calc_mode 1 Y_B->calc_mode 1
	    call NaF TABFILL Y {tab_xfills + 1} 0
    end

    if (!{exists Kdr})
	    create tabchannel Kdr
	    setfield Kdr Ek {EK} Gbar {GKdr} Ik 0 Gk 0 Xpower 4 Ypower 0 Zpower 0

		/////
		// The activation of Kdr
		///// 
	    call Kdr TABCREATE X {tab_xdivs} {tab_xmin}	{tab_xmax}
	    x = {tab_xmin}
	    dx = ({tab_xmax} - {tab_xmin})/{tab_xdivs}

	    for (i = 0; i <= ({tab_xdivs}); i = i + 1)
		    tauN = 2*NaFTauN/{{exp {(x-NaFThetaN)/(2*NaFSigmaN)}} + {exp {-(x-NaFThetaN)/(2*NaFSigmaN)}}}
			setfield Kdr X_A->table[{i}] {tauN}
			minfV = 1/{1 + {exp {(x-NaFThetaN)/NaFSigmaN}}}
		    setfield Kdr X_B->table[{i}] {minfV}
		    x = x + dx
	    end
	    tweaktau Kdr X
	    setfield Kdr X_A->calc_mode 1 X_B->calc_mode 1
	    call Kdr TABFILL X {tab_xfills + 1} 0
    end


	if (!{exists NaP})
	    create tabchannel NaP
	    setfield NaP Ek {ENa} Gbar {GNaP} Ik 0 Gk 0 Xpower 1 Ypower 1 Zpower 0

	    call NaP TABCREATE X {tab_xdivs} {tab_xmin} {tab_xmax}
	    x = {tab_xmin}
	    dx = ({tab_xmax} - {tab_xmin})/{tab_xdivs}
		
		/////
		// The activation of NaP
		/////
		setfield NaP instant {INSTANTX} 
	    for (i = 0; i <= ({tab_xdivs}); i = i + 1)
		    setfield NaP X_A->table[{i}] 1

			minfV = 1 + {exp {(x-NaPThetaM)/NaPSigmaM}}
		    setfield NaP X_B->table[{i}] {minfV}
		    x = x + dx
	    end
	    setfield NaP X_A->calc_mode 1 X_B->calc_mode 1
	    call NaP TABFILL X {tab_xfills + 1} 0
		
		/////
		// The inactivation of NaP
		///// 
	    call NaP TABCREATE Y {tab_xdivs} {tab_xmin}	{tab_xmax}
	    x = {tab_xmin}
	    dx = ({tab_xmax} - {tab_xmin})/{tab_xdivs}

	    for (i = 0; i <= ({tab_xdivs}); i = i + 1)
		    tauH = 2*NaPTauH/{{exp {(x-NaPThetaH)/(2*NaPSigmaH)}} + {exp {-(x-NaPThetaH)/(2*NaPSigmaH)}}}
			setfield NaP Y_A->table[{i}] {tauH}
			minfV = 1/{1 + {exp {(x-NaPThetaH)/NaPSigmaH}}}
		    setfield NaP Y_B->table[{i}] {minfV}
		    x = x + dx
	    end
	    tweaktau NaP Y
	    setfield NaP Y_A->calc_mode 1 Y_B->calc_mode 1
	    call NaP TABFILL Y {tab_xfills + 1} 0
    end

    pope
end



