count=0
for f in *.oa; do
    python ../assemble.py $f tmp.bc
    if [ ! -f tmp.bc ]; then
        echo "Failed to assemble $f"
        ((count++))
        continue
    fi
    output=$(../release/ovm tmp.bc 2>&1)
    expected=$(sed -n 's/^#\s*Expect:\s*\(.*\)$/\1/p' $f)
    if [ "$output" != "$expected" ]; then
        echo "FAIL: $f, expected $expected, but got $output"
        ((count++))
    fi
    rm tmp.bc
done

echo "$count failures"
