<?php

require "../core.php";

if(!isset($_POST["user"])) exit("Not set: user");
if(!isset($_POST["pass"])) exit("Not set: pass");

$user = $_POST["user"];
$pass = $_POST["pass"];

if($user == "") exit("Not valid: user");
if($pass == "") exit("Not valid: pass");

if(getUsersDB()->hasUser($user)) exit("Not valid: user exists already");

$newUser = [];
$newUser["passHash"] = hash("sha256", $pass);

getUsersDB()->addUser($user, $newUser);

header('Location: ../../registerSuccess.php');

?>