#!/usr/bin/env fish
set dir_path "$argv[1]"
set title_suffix "$argv[2]"

function run_npf_tests -a target_dir
    if not test -d "$target_dir"
        echo "Error: Directory '$target_dir' not found."
        return 1
    end

    for npf_file in $target_dir/*.npf
        if test -f "$npf_file"
            set filename (basename $npf_file)

            set base_name (string replace -r '\.npf$' '' $filename)

            if test "$base_name" = randr
                set graph_title "Random read throughput"
            else if test "$base_name" = read
                set graph_title "Sequential read throughput"
            else if test "$base_name" = randw
                set graph_title "Random write throughput"
            else if test "$base_name" = write
                set graph_title "Sequential write throughput"
            else
                set parsed_name (string replace -a '-' ' ' $base_name)
                set graph_title (string upper (string sub -s 1 -l 1 $parsed_name))(string sub -s 2 $parsed_name)" throughput"
            end

            if test -n "$title_suffix"
                set graph_title "$graph_title $title_suffix"
            end


            npf --test $npf_file --use-local $target_dir --single-output "$target_dir/csv/$base_name.csv" --graph-title "$graph_title" \
                --graph-filename "$target_dir/graphs/$base_name.pdf"
        end
    end
end

run_npf_tests "$dir_path"
