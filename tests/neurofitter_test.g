//genesis

// Based on Butera RJ, Rinzel J, Smith JC. (1999) 
// Models of respiratory rhythm generation in the pre-Botzinger complex. I. Bursting pacemaker neurons. J Neurophysiol 82:382-97

include const.g
include chansave.g

str cellpath =  "/neurofitter_test"
str paramfile = "param.dat"

str outputfile
float inject

function readParameters
	openfile {paramfile} r
	outputfile = {readfile {paramfile} -linemode}
	inject = {getarg {readfile {paramfile}} -arg 1}*1e-9
	str cond = {readfile {paramfile} -linemode}
	GNaP = {getarg {arglist {cond}} -arg 1}*1e-12
	GKdr = {getarg {arglist {cond}} -arg 2}*1e-12
end

function makeLibrary

	float len, dia, surf
	
	create neutral library

	pushe /library
	
	makeChannels
	
	// Create soma
	create compartment soma

	setfield soma Cm {CMs} Em {ELEAK} initVm {EREST_ACT} Rm {RMs}

	copy NaF soma/NaF
        addmsg soma soma/NaF VOLTAGE Vm                         
        addmsg soma/NaF soma CHANNEL Gk Ek
        setfield soma/NaF Gbar {GNaF}

	copy NaP soma/NaP
        addmsg soma soma/NaP VOLTAGE Vm                         
        addmsg soma/NaP soma CHANNEL Gk Ek
        setfield soma/NaP Gbar {GNaP}

	copy Kdr soma/Kdr
        addmsg soma soma/Kdr VOLTAGE Vm                         
        addmsg soma/Kdr soma CHANNEL Gk Ek
        setfield soma/Kdr Gbar {GKdr}

/*	create compartment dend1

	len = dend1Len
	dia = dend1Dia
	surf = PI*{dia}*{len}
	setfield dend1 Cm {{CMd}*{surf}} Ra {{RA}*{len}/(dia*dia*{PI})}  \
		Em {ELEAK} initVm {EREST_ACT} Rm {{RMd}/{surf}} inject 0  \
		dia {dia} len {len}
	*/
/*	copy NaF dend1/NaF
        addmsg dend1 dend1/NaF VOLTAGE Vm                         
        addmsg dend1/NaF dend1 CHANNEL Gk Ek
        setfield dend1/NaF Gbar {GNaF}

	copy Kdr dend1/Kdr
        addmsg dend1 dend1/Kdr VOLTAGE Vm                         
        addmsg dend1/Kdr dend1 CHANNEL Gk Ek
        setfield dend1/Kdr Gbar {GKdr}


	copy KA dend1/KA
        addmsg dend1 dend1/KA VOLTAGE Vm                         
        addmsg dend1/KA dend1 CHANNEL Gk Ek
        setfield dend1/KA Gbar {GKA}

*/
	pope
end

function makeCell(cellpath)
		
	create neutral {cellpath}
	
	pushe {cellpath}

	copy /library/soma soma
	//copy /library/dend1 dend1

	setfield soma Em {ELEAK}

	//addmsg soma dend1 AXIAL previous_state
    //addmsg dend1 soma RAXIAL Ra previous_state


	pope
end

function makeOutput(filename, cellpath) 

	create asc_file /output/plot_out
	useclock ^ 9

	setfield ^ filename {outputfile} initialize 1 append 1 flush 0 leave_open 1

	addmsg {cellpath}/soma /output/plot_out SAVE Vm
	//addmsg {cellpath}/soma/NaP /output/plot_out SAVE Y
	//addmsg {cellpath}/dend1 /output/plot_out SAVE Vm
	
end


function makeInjection(cellpath, cellpart, inject)

	setfield {cellpath}/{cellpart} inject {inject}

end

readParameters

//Create the library with all the cell elements
echo "Making compartment library"
makeLibrary

//Create the cell
echo "Making cell"
makeCell {cellpath}


//Create injection
echo "Making injection"
makeInjection {cellpath} soma {inject}

makeOutput {outputfile} {cellpath}


int i
	
for (i = 0 ; i <= 9 ; i = i + 1) 
   	setclock {i} {dt}
end

reset

echo "Running simulation"
makeInjection {cellpath} soma {inject}
step {4} -time

exit
