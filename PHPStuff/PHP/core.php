<head>
	<link rel="stylesheet" type="text/css" href="./CSS/style.css">
</head>

<?php 

ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(-1);
date_default_timezone_set("UTC");
session_start();

class JsonDB
{
	private $fileName;
	protected $obj;

	public function __construct($mFileName)
	{
		$this->fileName = $mFileName;
		$this->readFromFile();
	}

	protected function writeToFile() 
	{ 
		$contents = json_encode($this->obj);
		file_put_contents($this->fileName, $contents);

	}

	protected function readFromFile() 
	{ 
		$contents = file_get_contents($this->fileName);
		$this->obj = json_decode($contents, true);		
	}
}

class PostsDB extends JsonDB
{
	public function __construct()
	{
		parent::__construct('/srv/http/Data/posts.json');
	}

	private function initComments($mPostID)
	{
		if(!isset($this->obj[$mPostID]["comments"]))
		{
			$this->obj[$mPostID]["comments"] = [];
		}
	}

	public function getPosts()
	{
		return $this->obj;
	}
	
	public function getPost($mPostID)
	{
		return $this->obj[$mPostID];
	}

	public function addComment($mPostID, $mComment)
	{
		$this->initComments($mPostID);
		array_push($this->obj[$mPostID]["comments"], $mComment);
		$this->writeToFile();
	}

	public function addPost($mPost)
	{
		array_push($this->obj, $mPost);
		$this->writeToFile();
	}
}

class UsersDB extends JsonDB
{
	public function __construct()
	{
		parent::__construct('/srv/http/Data/users.json');
	}

	public function addUser($mUser, $mUserData)
	{
		$this->obj[$mUser] = $mUserData;
		$this->writeToFile();	
	}

	public function hasUser($mUser)
	{
		if(!isset($this->obj[$mUser])) return false;
		return true;
	}

	public function isUserPassHash($mUser, $mPassHash)
	{
		return $this->obj[$mUser]["passHash"] == $mPassHash;
	}
}

function getUsersDB() { return new UsersDB(); }
function getPostsDB() { return new PostsDB(); }

class Credentials
{
	static public function isLoggedIn()
	{
		if(!isset($_SESSION["currentUser"])) return false;
		if($_SESSION["currentUser"] == "") return false;
		return true;
	}

	static public function login($mUser)
	{
		$_SESSION["currentUser"] = $mUser;
	}

	static public function logout()
	{
		unset($_SESSION["currentUser"]);
	}

	static public function getCurrentUserName()
	{
		return $_SESSION["currentUser"];
	}
}	

class Builder
{
	static public function printCommon()
	{
		Builder::printMenu();
		Builder::printLoginStatus();		
	}

	static public function printLoginStatus()
	{
		print('<div class="loginStatus">');
		if(Credentials::isLoggedIn())
		{
			print("Logged in as: " . Credentials::getCurrentUserName());
			print('
			<form action="./PHP/Actions/doLogout.php" method="post">
				<p><button type="submit">Logout</button></p>
			</form>
			');
		}
		else
		{
			print("Currently not logged in.");
		}
		print('</div>');
	}

	static public function printPost($mPost, $mPostID)
	{
		print('<div class="post">');

		print("<h1>");	
		Builder::printLink($mPost["title"], './viewPost.php?postID=' . $mPostID);
		print("</h1>");

		print("<strong>");	
		print($mPost["author"]);
		print("</strong>");

		print(" || ");		

		print("<strong>");	
		print($mPost["date"]);
		print("</strong>");	

		print("</br>");			

		print("<p>");
		print($mPost["contents"]);
		print("</p>");

		print("<hr>");

		print('</div>');
	}

	static public function printLink($mName, $mLink)
	{
		print('<a href="' . $mLink . '">' . $mName . '</a>');
	}

	static public function printMenuItem($mName, $mLink)
	{
		print("<li>");
		Builder::printLink($mName, $mLink);
		print("</li>");
	}

	static public function printMenu()
	{
		print('<div class="mainMenu">');
		print("<ul>");
		Builder::printMenuItem("Index", './index.php');
		Builder::printMenuItem("Create post", './createPost.php');
		Builder::printMenuItem("Register/login", './registerLogin.php');
		print("</ul>");		
		print('</div>');
	}

	static public function printComment($mComment)
	{
		print("<li>");
		print("<p>Author: " . $mComment["author"] . "</p>");
		print("<p> " . $mComment["contents"] . "</p>");
		print("</li>");
	}

	static public function printPostComments($mPost, $mPostID)
	{
		if(!isset($mPost["comments"]) || count($mPost["comments"]) == 0)
		{
			return;
		}

		print("<h3>Comments:</h3>");

		print("<ul>");
		foreach($mPost["comments"] as $c)
		{
			Builder::printComment($c);
		}	
		print("</ul>");
	}
}

?>