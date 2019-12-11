INPUTS=$(ls *_input.txt | grep -v "result_")
rm -f result_*
if [ $# -ne 0 ]
then
	echo "Start testing..."
	for input in $INPUTS
	do
		echo "teting for "$input
		for arg in $*
		do
			./[Binary File] $input $arg
		done
	done
	echo "Finish testing!!!"
else 
	echo "Program Manual - sh tester.sh [@algorithm_index]"
	echo "please write algorithm_index"
fi
