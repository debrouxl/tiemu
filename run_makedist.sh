#! /bin/sh
for module in skinedit tiemu; do
echo Updating "$module"
cd "$module/trunk"; make dist || exit 1; cd ../..
done
