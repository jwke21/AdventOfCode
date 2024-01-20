use std::fs;
use std::io::{self, BufRead};
use std::path::Path;

fn main() {
    println!("Reading input...");

    let mut sum = 0;

    if let Ok(lines) = read_lines("input") {
        // consumes the iterator, returns an (Optional) String
        for line in lines.flatten() {
            let mut first = ' ';
            let mut second = '0';

            for (index, c) in line.char_indices() {
                // get the first and last numbers
                if c.is_digit(10) {
                    if first == ' ' {
                        first = c;
                    }
                    second = c;
                }
                // check if it is a spelled out number
                else if c == 'o' || c == 't' || c == 'f' || c == 's' || c == 'e' || c == 'n' {
                    let num = is_spelled_number(&line, index);

                    if num != ' ' {
                        if first == ' ' {
                            first = num;
                        }
                        second = num;
                    }
                }
            }

            // add the calibration value to the running sum
            let combined = format!("{first}{second}");
            // let n1 = first.to_digit(10).unwrap();
            // let n2 = second.to_digit(10).unwrap();
            // sum += n1 * 10 + n2;
            let n = combined.parse::<u32>().unwrap();
            // println!("{} + {} = {}", sum, n, sum + n);
            sum += n;
            // println!("{sum}");
            // println!("{line}");
        }
    }

    println!("Sum of all calibration values: {sum}");

    // println!("\n{contents}");
}

/**
   Function to read the lines of a file. Returns an iterator over the lines.
*/
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<fs::File>>>
where
    P: AsRef<Path>,
{
    let file = fs::File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn is_spelled_number(line: &String, start_idx: usize) -> char {
    // check if length three word is valid
    if start_idx + 3 <= line.chars().count() {
        let three_word = &line[start_idx..start_idx + 3];

        if three_word == String::from("one") {
            return '1';
        } else if three_word == String::from("two") {
            return '2';
        } else if three_word == String::from("six") {
            return '6';
        }

        // check if length four word can be formed
        if start_idx + 4 <= line.chars().count() {
            let four_word = &line[start_idx..start_idx + 4];

            if four_word == String::from("four") {
                return '4';
            } else if four_word == String::from("five") {
                return '5';
            } else if four_word == String::from("nine") {
                return '9';
            }

            // check if length five word can be formed
            if start_idx + 5 <= line.chars().count() {
                let five_word = &line[start_idx..start_idx + 5];

                if five_word == String::from("three") {
                    return '3';
                } else if five_word == String::from("seven") {
                    return '7';
                } else if five_word == String::from("eight") {
                    return '8';
                }
            }
        }
    }

    ' '
}
