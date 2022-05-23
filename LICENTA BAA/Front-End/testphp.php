<?php
error_reporting(0);
if( $_POST["name"] || $_POST["weight"] )
	{
		if (preg_match("/[^A-Za-z'-]/",$_POST['name'] ))
		{
		die ("invalid name and name should be alpha");
		}
	echo "Welcome ". $_POST['name']. "<br />";
	echo "You are ". $_POST['weight']. " kgs in weight.";
	exit();
		}
?>
<html>
<body>
<form action = "<?php $_PHP_SELF ?>" method = "POST">
	Name: <input type = "text" name = "name" />
	Weight: <input type = "text" name = "weight" />
			<input type = "submit" />
</form>
</body>
</html>
