<?php

require "../core.php";

if(!Credentials::isLoggedIn()) exit("Must be logged in.");
if(!isset($_POST["contents"])) exit("Not set: contents");
if(!isset($_SESSION["postID"])) exit("Not set: postID");

$date = date("d/m/Y");
$contents = $_POST["contents"];
$postID = $_SESSION["postID"];

if($contents == "") exit("Not valid: contents");
if($postID == "") exit("Not valid: postID");

$newComment = [];
$newComment["author"] = Credentials::getCurrentUserName();
$newComment["date"] = $date;
$newComment["contents"] = nl2br($contents);

getPostsDB()->addComment($postID, $newComment);

header('Location: ../../viewPost.php?postID=' . $postID);

?>