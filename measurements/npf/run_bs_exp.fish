#!/usr/bin/env fish
set dir_path "$argv[1]"

function run_npf_tests -a target_dir
    if not test -d "$target_dir"
        echo "Error: Directory '$target_dir' not found."
        return 1
    end

    for npf_file in $target_dir/*.npf
        if test -f "$npf_file"
            set filename (basename $npf_file)

            set base_name (string replace -r '\.npf$' '' $filename)

            set parsed_name (string replace -a '-' ' ' $base_name)
            set graph_title (string upper (string sub -s 1 -l 1 $parsed_name))(string sub -s 2 $parsed_name)" throughput (D = I/O Depth, S = file size)"

            npf --test $npf_file --use-local $target_dir --single-output "$base_name.csv" --graph-title "$graph_title"
        end
    end
end

run_npf_tests "$dir_path"
