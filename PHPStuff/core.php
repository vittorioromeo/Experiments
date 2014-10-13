<?php 

date_default_timezone_set("UTC");

class BlogPost implements JsonSerializable 
{
	private $title = "Untitled blog post";
	private $author = "Anonymous";
	private $date;
	private $contents = "...";

	// function __construct() { }
	// function __destruct() { }

	public function jsonSerialize() 
	{
        return 
        [
            "title" => $this->title,
            "author" => $this->author,
            "date" => $this->date,
            "contents" => $this->contents
        ];
    }

	public static function fromJson($j)
	{
		$result = new BlogPost();

		$result->title = $j->title;
		$result->author = $j->author;
		$result->date = $j->date;
		$result->contents = $j->contents;

		return $result;
	}

	public function toJson()
	{
		return json_encode($this);
	}

	public function setTitle($v)	{ $this->title = $v; }
	public function setAuthor($v)	{ $this->author = $v; }
	public function setDate($v)		{ $this->date = $v; }
	public function setContents($v)	{ $this->contents = $v; }

	public function getTitle() 		{ return $this->title; }
	public function getAuthor() 	{ return $this->author; }
	public function getDate() 		{ return $this->date; }
	public function getContents() 	{ return $this->contents; }
}

class BlogBuilder
{
	static public function printPost($post)
	{
		print("<h1>");	
		print($post->getTitle());
		print("</h1>");


		print("<strong>");	
		print($post->getAuthor());
		print("</strong>");

		print(" || ");		

		print("<strong>");	
		print($post->getDate());
		print("</strong>");	

		print("</br>");			

		print("<p>");
		print($post->getContents());
		print("</p>");

		print("<hr>");
	}
}

function readPostsToArray()
{
	$postsFileContents = file_get_contents("./posts.json");
	$postsJsonArray = json_decode($postsFileContents);

	$result = [];

	foreach($postsJsonArray as $p)
	{
		array_push($result, BlogPost::fromJson($p));
	}

	return $result;
}

function writePostsToFile($posts)
{
	$str = json_encode($posts);
	file_put_contents("./posts.json", $str);
}

?>