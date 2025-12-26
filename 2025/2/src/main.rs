use std::fs::File;
use std::io::{
    BufRead, BufReader,
};

fn main() {
    // read input from arguments or default to input.txt
    let args: Vec<String> = std::env::args().collect();
    let input_path = if args.len() > 1 {
        &args[1]
    } else {
        "input.txt"
    };

    let line= read_line(input_path).
        expect("Unable to read file lines");
    let ranges = line.
        split(",").
        collect::<Vec<&str>>();
    // println!("Parsed ranges: {:?}", ranges);

    let mut total_invalid_part1 = 0;
    let mut total_invalid_part2 = 0;
    for range in ranges {
        let bounds = range.
            split("-").
            collect::<Vec<&str>>();

        if bounds.len() != 2 {
            panic!("Unable to parse range {}", range);
        }

        let start: u64 = bounds[0].
            parse().
            expect(format!("Unable to parse start {}", bounds[0]).as_str());
        let end: u64 = bounds[1].
            parse().
            expect(format!("Unable to parse end {}", bounds[1]).as_str());

        let (invalid_sum_part1, invalid_sum_part2) = sum_invalid_ids(start, end);
        println!("Range from {} to {} has invalid IDs with sum {} (part 1) and {} (part 2)", start, end, invalid_sum_part1, invalid_sum_part2);
        total_invalid_part1 += invalid_sum_part1;
        total_invalid_part2 += invalid_sum_part2;
    }

    println!("\nTotal invalid IDs for part 1: {}", total_invalid_part1);
    println!("Total invalid IDs for part 2: {}", total_invalid_part2);
}

fn read_line(path: &str) -> std::io::Result<String> {
    let file = File::open(path).
        expect("Unable to open file");
    let mut buf = BufReader::new(file);

    let mut line = String::new();
    buf.read_line(&mut line)?;
    Ok(line)
}

fn sum_invalid_ids(start: u64, end: u64) -> (u64, u64) {
    let mut invalid_sum_part1 = 0;
    let mut invalid_sum_part2 = 0;
    for id in start..=end {
        let id_str = id.to_string();
        if is_invalid_id_part1(&id_str) {
            invalid_sum_part1 += id;
            invalid_sum_part2 += id;
            continue;
        }
        if is_invalid_id_part2(&id_str) {
            invalid_sum_part2 += id;
        }
    }
    (invalid_sum_part1, invalid_sum_part2)
}

/// Checks if the ID is composed of two equivalent halves.
/// 
/// e.g. 123123, 456456 both return true
///     123456, 111222 both return false
fn is_invalid_id_part1(id_str: &String) -> bool {
    let length = id_str.chars().count();
    if length % 2 != 0 {
        return false;
    }
    let half = length / 2;
    let start_str = &id_str[0..half];
    let end_str = &id_str[half..length];

    let start: u64 = start_str.
        parse().
        expect(format!("Unable to parse start half {}", start_str).as_str());
    let end: u64 = end_str.
        parse().
        expect(format!("Unable to parse end half {}", end_str).as_str());

    start == end
}

/// Checks if the ID is composed of any number of duplicated substrings
///
/// e.g. 1212 (12 duplicated twice), 111 (1 duplicated 3 times) both return true
///    12312, 1231234 both return false
fn is_invalid_id_part2(id_str: &String) -> bool {
    let length = id_str.chars().count();
    let mut cur_pos = 0;
    while cur_pos <= length / 2 {
        let substr = &id_str[0..cur_pos + 1];
        let substr_len = substr.chars().count();
        // println!("Checking substring: {}", substr);

        // check if the string can be constructed by repeating substr
        if length % substr_len != 0 || length == substr_len{
            cur_pos += 1;
            continue;
        }

        let sub: u64 = substr.to_string().
            parse().
            expect(format!("Unable to parse substring {}", substr).as_str());

        let mut all_match = true;
        for cur_substr_start in (cur_pos + 1..length).step_by(substr_len) {
            let cur_substr = &id_str[cur_substr_start..cur_substr_start + substr_len];
            // println!("Comparing to substring: {}", cur_substr);
            let cur: u64 = cur_substr.to_string().
                parse().
                expect(format!("Unable to parse current substring {}", cur_substr).as_str());
            if cur != sub {
                all_match = false;
                break;
            }
        }

        if all_match {
            // println!("All substrings match for substring {}", substr);
            return true;
        }

        cur_pos += 1;
    }

    false
}

