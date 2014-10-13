<?php

require "../core.php";

if(!Credentials::isLoggedIn()) exit("Must be logged in.");
if(!isset($_POST["title"])) exit("Not set: title");
if(!isset($_POST["contents"])) exit("Not set: contents");

$title = $_POST["title"];
$date = date("d/m/Y");
$contents = $_POST["contents"];

if($title == "") exit("Not valid: title");
if($contents == "") exit("Not valid: contents");

$newPost = [];
$newPost["title"] = $title;
$newPost["author"] = Credentials::getCurrentUserName();
$newPost["date"] = $date;
$newPost["contents"] = nl2br($contents);

getPostsDB()->addPost($newPost);

header('Location: ../../index.php');

?>