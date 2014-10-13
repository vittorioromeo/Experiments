<?php

require "../core.php";

if(!isset($_POST["user"])) exit("Not set: user");
if(!isset($_POST["pass"])) exit("Not set: pass");

$user = $_POST["user"];
$pass = $_POST["pass"];

if($user == "") exit("Not valid: user");
if($pass == "") exit("Not valid: pass");

$passHash = hash("sha256", $pass);

$usersDB = getUsersDB();

if($usersDB->hasUser($user))
{
	if($usersDB->isUserPassHash($user, $passHash))	
	{
		Credentials::login($user);
		header('Location: ../../loginSuccess.php');
	}
	else
	{
		print("Wrong password");
	}
}
else
{
	print("No user with name: " . $user);
}

?>