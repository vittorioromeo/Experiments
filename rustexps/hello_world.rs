use std::io;

fn main() 
{
	let mut guess = String::new();
	io::stdin().read_line(&mut guess)
		.ok()
		.expect("Failed to read line!");

	println!("{}", guess);
}