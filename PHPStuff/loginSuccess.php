<?php

require "./PHP/core.php";

Builder::printCommon();

if(Credentials::isLoggedIn())
{
	print("Error: already logged in.");
}
else
{
	print("<h2>Login successful. Welcome, " . Credentials::getCurrentUserName() . "</h2>");
}

?>