INPUTS=$(ls *_input.txt)
rm -f *input.txt.zz *input.txt.zz.yy

echo "Start compressing ..."
for input in $INPUTS
do
	echo "Compress the "$input
	./[Binary File] -c $input
done
echo "Finish compressing !"


INPUTS=$(ls *_input.txt.zz)

echo "Start decompressing ..."
for input in $INPUTS
do
	echo "Decompress the "$input
	./[Binary File] -d $input
done
echo "Finish decompressing !"
