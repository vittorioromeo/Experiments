<?php

require "./core.php";

ini_set('display_errors',1);
ini_set('display_startup_errors',1);
error_reporting(-1);

$posts = readPostsToArray();

$postId = isset($_GET["postId"]) ? $_GET["postId"] : null;
print($postId);

if($postId == null)
{
	$id = 0;

	foreach($posts as $p)
	{
		print("Post ID: ");
		print($id++);
		BlogBuilder::printPost($p);
	}

	print('
	<form action="newPost.php" method="post">
		<p>Post title: <input type="text" name="title" /></p>
		<p>Post author: <input type="text" name="author" /></p>
		<p>Post contents: <input type="text" name="contents" /></p>
		<p><input type="submit" /></p>
	</form>
	');
}
else
{
	print("<h2>Looking at post with ID: ");
	print($postId);
	print("</h2>");

	BlogBuilder::printPost($posts[$postId]);
}


?>