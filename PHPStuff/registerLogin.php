<?php

require "./PHP/core.php";

Builder::printCommon();

if(!Credentials::isLoggedIn())
{
	print("<h2>Login:</h2>");

	print('
	<form action="./PHP/Actions/doLogin.php" method="post">
		<p>User: <input type="text" name="user" /></p>
		<p>Pass: <input type="password" name="pass" /></p>
		<p><input type="submit" /></p>
	</form>
	');

	print("<hr>");

	print("<h2>Register:</h2>");

	print('
	<form action="./PHP/Actions/doRegister.php" method="post">
		<p>User: <input type="text" name="user" /></p>
		<p>Pass: <input type="password" name="pass" /></p>
		<p><input type="submit" /></p>
	</form>
	');
}
else
{
	print("Already logged in.");
}

?>