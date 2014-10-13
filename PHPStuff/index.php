<?php

require "./PHP/core.php";

Builder::printCommon();

$id = 0;

foreach(getPostsDB()->getPosts() as $p)
{
	print("Post ID: ");
	print($id);
	
	Builder::printPost($p, $id);

	++$id;
}

?>