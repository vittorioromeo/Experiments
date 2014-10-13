<?php

require "./PHP/core.php";

Builder::printCommon();

if(Credentials::isLoggedIn())
{
	print('
	<form action="./PHP/Actions/newPost.php" method="post">
		<p>Post title: <input type="text" name="title" /></p>
		<p>Post contents: <div class="contents"><textarea name="contents" rows="5" cols="40"></textarea></div></p>
		<p><input type="submit" /></p>
	</form>
	');
}
else
{
	print("Must be logged in to create posts.");
}

?>