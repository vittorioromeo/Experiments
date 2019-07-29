#![feature(custom_attribute)]
#![feature(stmt_expr_attributes)]

use std::io;
use std::io::Read;

#[must_use] // ANNOYANCE: not default
#[repr(u8)] // ANNOYANCE: ugly syntax compared to enum class
#[derive(PartialEq, Copy, Clone)]
enum Tile {
    None,
    Wall,
    Goal,
}

#[must_use]
#[repr(u8)]
#[derive(PartialEq, Copy, Clone)]
enum Obj {
    None,
    Player,
    Box,
}

const BOARD_WIDTH: usize = 8;
const BOARD_HEIGHT: usize = 8;

type Layer<T> = [T; BOARD_WIDTH * BOARD_HEIGHT];

type Index = usize;
type Coord = usize;
type Vec2D = (Coord, Coord);

#[must_use]
const fn to_index((x, y): Vec2D) -> Index {
    (y * BOARD_WIDTH) + x
}

#[must_use]
const fn to_vec2d(i: Index) -> Vec2D {
    (i % BOARD_WIDTH, i / BOARD_WIDTH)
}

#[must_use]
fn tile_char(tile: Tile) -> char {
    // ANNOYANCE: Can't be `const fn`
    match tile {
        Tile::None => ' ',
        Tile::Wall => '▒',
        Tile::Goal => '○',
    }
}

#[must_use]
fn obj_char(obj: Obj, tile: Tile) -> char {
    // ANNOYANCE: Can't be `const fn`
    match obj {
        Obj::None => tile_char(tile),
        Obj::Player => '☻',
        Obj::Box => {
            if tile == Tile::Goal {
                '◙'
            } else {
                '■'
            }
        } // ANNOYANCE: verbose, ugly
    }
}

#[must_use]
struct Board {
    tiles: Layer<Tile>,
    objects: Layer<Obj>,
}

impl Board {
    fn print(&self) {
        for y in 0..BOARD_HEIGHT {
            for x in 0..BOARD_WIDTH {
                let i = to_index((x, y));
                print!("{}", obj_char(self.objects[i], self.tiles[i]));
            }
            println!();
        }
    }

    #[must_use]
    fn find_player(&self) -> Coord {
        self.objects.iter().position(|x| *x == Obj::Player).unwrap()
    }

    #[must_use]
    fn count_goals(&self) -> usize {
        self.tiles.iter().filter(|x| **x == Tile::Goal).count() // TODO: why all the stars?
    }
}

#[must_use]
struct Game {
    board: Board,
    player_index: Coord,
    goals_left: usize,
}

impl Game {
    #[must_use]
    fn new(board: Board) -> Game {
        Game {
            player_index: board.find_player(),
            goals_left: board.count_goals(),
            board,
        }
    }

    #[must_use]
    fn obj_at(&mut self, i: Index) -> &mut Obj {
        &mut self.board.objects[i]
    }

    #[must_use]
    fn tile_at(&mut self, i: Index) -> &mut Tile {
        &mut self.board.tiles[i]
    }

    #[must_use]
    fn move_box(&mut self, pos: Vec2D, (ox, oy): (isize, isize)) -> bool {
        let (px, py) = pos;
        let source = to_index((px, py));
        let target = to_index(((px as isize + ox) as usize, (py as isize + oy) as usize));

        if *self.tile_at(target) == Tile::Wall || *self.obj_at(target) != Obj::None {
            return false;
        }

        if *self.tile_at(source) == Tile::Goal {
            self.goals_left += 1;
        }

        if *self.tile_at(target) == Tile::Goal {
            self.goals_left -= 1;
        }

        self.board.objects.swap(target, source);
        true
    }

    fn move_player(&mut self, offset: (isize, isize)) -> bool {
        let (px, py) = to_vec2d(self.player_index);
        let (ox, oy) = offset;
        // TODO: can the many casts be avoided?
        let (tx, ty): (isize, isize) = (px as isize + ox, py as isize + oy);
        let target_vec2d = (tx as usize, ty as usize);
        let target = to_index(target_vec2d);

        let couldnt_push_box =
            *self.obj_at(target) == Obj::Box && !self.move_box(target_vec2d, offset);

        if *self.tile_at(target) == Tile::Wall || couldnt_push_box {
            return false;
        }

        self.board.objects.swap(target, self.player_index);
        self.player_index = target;
        true
    }

    fn print(&self) {
        self.board.print();
        println!("\nGoals left: {}\n", self.goals_left);
    }
}

static TILE_LAYER: Layer<Tile> = {
    #[allow(non_snake_case)]
    let (o, H, X) = (Tile::None, Tile::Wall, Tile::Goal);

    #[rustfmt::skip]
    [H,H,H,H,H,H,H,H,
     H,H,o,o,o,o,o,H,
     H,o,o,o,o,o,o,H,
     H,o,o,o,o,o,o,H,
     H,o,o,o,H,o,X,H,
     H,o,o,o,o,o,X,H,
     H,o,o,o,X,X,X,H,
     H,H,H,H,H,H,H,H]
};

static OBJECT_LAYER: Layer<Obj> = {
    #[allow(non_snake_case)]
    let (o, P, B) = (Obj::None, Obj::Player, Obj::Box);

    #[rustfmt::skip]
    [o,o,o,o,o,o,o,o,
     o,o,o,o,o,o,o,o,
     o,o,B,B,o,o,o,o,
     o,o,B,o,B,o,o,o,
     o,o,o,o,o,o,o,o,
     o,o,o,o,B,o,o,o,
     o,P,o,o,o,o,o,o,
     o,o,o,o,o,o,o,o]
};

#[must_use]
fn restart() -> bool {
    let mut game = Game::new(Board {
        tiles: TILE_LAYER,
        objects: OBJECT_LAYER,
    });

    loop {
        let _ = std::process::Command::new("clear").status();
        game.print();

        let input = io::stdin().lock().bytes().nth(0).unwrap().unwrap() as char;

        #[rustfmt::skip]
        match input as char {
            'w' => { game.move_player(( 0, -1)); }
            's' => { game.move_player(( 0,  1)); }
            'a' => { game.move_player((-1,  0)); }
            'd' => { game.move_player(( 1,  0)); }
            _   => {}
        }

        if input == 'r' {
            break true;
        }
        if input == 'q' {
            break false;
        }
    }
}

fn main() {
    while restart() {}
}
