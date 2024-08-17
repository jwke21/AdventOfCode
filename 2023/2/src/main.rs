use std::fs;
use std::io::{self, BufRead};
use std::path::Path;
use std::cmp;

const MAX_RED: u32 = 12;
const MAX_GREEN: u32 = 13;
const MAX_BLUE: u32 = 14;

struct Game {
    game_id: u32,
    is_possible: bool,
    min_red: u32,
    min_green: u32,
    min_blue: u32,
}

fn main() {
    let mut possible_game_id_sum = 0;
    let mut power_sum = 0;
    if let Ok(lines) = read_lines("input.txt") {
        // handle each line
        for line in lines.flatten() {
            let game = handle_line(line);

            if game.is_possible {
                possible_game_id_sum += game.game_id;
            }

            let power = game.min_red * game.min_green * game.min_blue;
            power_sum += power;
        }
    }

    println!("Sum of possible game ids: {possible_game_id_sum}");
    println!("Sum of the power of the sets {power_sum}")
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<fs::File>>>
where
    P: AsRef<Path>,
{
    let file = fs::File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

// gets the max number of each color used for given game
fn handle_line(line: String) -> Game {
    let (id, start) = get_game_id(&line);

    println!("Checking game number {}", id);

    let mut game_res = Game{
        game_id: id,
        is_possible: true,
        min_red: 0,
        min_green: 0,
        min_blue: 0,
    };

    // get each set in the line
    let sets: Vec<&str> = line[start..].split("; ").collect();
    for set in sets {
        let mut num_reds = 0;
        let mut num_greens = 0;
        let mut num_blues = 0;

        // get the different colors
        let colors: Vec<&str> = set.split(", ").collect();
        for color in colors {
            let num_and_color: Vec<&str> = color.split(" ").collect();
            let num = num_and_color[0].parse::<u32>().unwrap();
            let color = num_and_color[1];

            match color {
                "red" => {
                    num_reds += num;
                    game_res.min_red = cmp::max(num_reds, game_res.min_red)
                },
                "green" => {
                    num_greens += num;
                    game_res.min_green = cmp::max(num_greens, game_res.min_green)
                },
                _ => {
                    num_blues += num;
                    game_res.min_blue = cmp::max(num_blues, game_res.min_blue)
                },
            }
        }

        if num_reds > MAX_RED {
            println!("Game {id} has too many reds: had {num_reds}, max {MAX_RED}");
            game_res.is_possible = false;
        } else if num_greens > MAX_GREEN {
            println!("Game {id} has too many greens: had {num_greens}, max {MAX_GREEN}");
            game_res.is_possible = false;
        } else if num_blues > MAX_BLUE {
            println!("Game {id} has too many blues: had {num_blues}, max {MAX_BLUE}");
            game_res.is_possible = false;
        }
    }

    return game_res
}

fn get_game_id(line: &String) -> (u32, usize) {
    // id starts at idx 5 (after "Game ")
    let (start, mut end) = (5, 0);
    let new_string = &line[5..];
    for (i, c) in new_string.char_indices() {
        if c == ':' {
            end = start + i;
            break
        }
    }

    // get the id
    let id = line[start..end].parse::<u32>().unwrap();
    return (id, end + 2);
}
