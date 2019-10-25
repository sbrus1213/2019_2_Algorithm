INPUTS=$(ls *_input.txt | grep -v "result")
rm -f result*
if [ $# -ne 0 ]
then
	echo "Start testing..."
	for input in $INPUTS
	do
		for arg in $*
		do
			./mp2_20141284 $input $arg
		done
		echo "make result file for "$input
	done
	echo "Finish testing!!!"
else 
	echo "Program Manual - sh tester.sh [@algorithm_index]"
	echo "please write algorithm_index"
fi
