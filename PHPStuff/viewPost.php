<?php

require "./PHP/core.php";

Builder::printCommon();

$postID = isset($_GET["postID"]) ? $_GET["postID"] : null;

if($postID === null)
{
	print("Invalid post ID.");
}
else
{
	$post = getPostsDB()->getPost($postID);
	$_SESSION["postID"] = $postID;

	Builder::printPost($post, $postID);

	if(Credentials::isLoggedIn())
	{
		print('
		<form action="./PHP/Actions/newComment.php" method="post">		
			<p>Comment contents: <div class="contents"><textarea name="contents" rows="3" cols="40"></textarea></div></p>
			<p><input type="submit" /></p>
		</form>
		');
	}
	else
	{
		print("Must be logged in to create comments.");
	}

	Builder::printPostComments($post, $postID);
}

?>