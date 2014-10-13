<?php

require "./core.php";

$posts = readPostsToArray();

if(!isset($_POST["title"])) exit("Not set: title");
if(!isset($_POST["author"])) exit("Not set: author");
if(!isset($_POST["contents"])) exit("Not set: contents");

$title = $_POST["title"];
$author = $_POST["author"];
$date = date("d/m/Y");
$contents = $_POST["contents"];

if($title == "") exit("Not valid: title");
if($author == "") exit("Not valid: author");
if($contents == "") exit("Not valid: contents");

$newPost = new BlogPost();
$newPost->setTitle($title);
$newPost->setAuthor($author);
$newPost->setDate($date);
$newPost->setContents($contents);

array_push($posts, $newPost);

writePostsToFile($posts);

header('Location: ./index.php');

?>