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
    let lines= read_file_lines(input_path).
        expect("Unable to read file lines");
    
    let mut cur = 50;
    let mut total = 0;
    let mut clicks = 0;
    for line in lines {
        println!("Current dial position: {}, current line {}", cur, line);
        let direction = line.
            chars().
            next().
            expect(format!("Unable to get direction {}", line).as_str()).
            to_string();
        let amount: i32 = line[1..].
            parse().
            expect(format!("Unable to parse amount {}", line).as_str());
        let (new_cur, num_clicks) = rotate_dial(direction, cur, amount);
        cur = new_cur;
        clicks += num_clicks;
        println!("number of clicks this rotation: {}", num_clicks);
        if cur == 0 {
            total += 1;
        }
    }
    println!("Final dial position: {}", cur);
    println!("Total times dial reached 0: {}", total);
    println!("Total clicks: {}", clicks);
}

fn read_file_lines(path: &str) -> std::io::Result<Vec<String>> {
    let file = File::open(path).
        expect("Unable to open file");
    let buf = BufReader::new(file);

    let mut data = Vec::new();
    for line in buf.lines() {
        data.push(line?);
    }
    Ok(data)
}

// returns the new dial position after rotation
fn rotate_dial(direction: String, cur: i32, amount: i32) -> (i32, i32) {
    let mut new = cur;
    let mut amount = amount;
    let mut clicks = 0;
    while amount > 0 {
        if direction == "L" {
            if new == 0 {
                new = 99;
            }
            else {
                new -= 1;
            }
        } else {
            if new == 99 {
                new = 0;
            }
            else {
                new += 1;
            }
        }
        amount -= 1;
        if new == 0 {
            clicks += 1;
        }
    }
    (new, clicks)
}
