<!DOCTYPE html>
<html lang="ja">
	<head>
		<title>実装Ｂの結果</title>
	</head>
	<body bgcolor=yellow>
<?php

require 'geotag.php';

if(isset($_REQUEST["tag"])){
	$tag = $_REQUEST["tag"];
	$servername = "localhost";
	$username = "cs17045";
	$password = "password";
	$dbname = "mydatabase";
	$db = new geotag($servername, $username, $password, $dbname);
	$result = $db->search_address_by_postal($tag);
?>
	<table border="1" width="1000" cellspacing="0" cellpadding="5">
 		<thead>
  		<th bgcolor=#EE0000 scope="col">image</th>
  		<th bgcolor=#EE0000 scope="col">time</th>
  		<th bgcolor=#EE0000 scope="col">latitude</th>
  		<th bgcolor=#EE0000 scope="col">longitude</th>
		<th bgcolor=#EE0000 scope="col">url</th>
 		</thead>
 		<tbody>
		<?php
		$i = 0;
		foreach( $result as $row ) {
		if($i >= 100)break;
		?>
  		<tr>
		<td><img src=<?php echo $row[3] ?> width=100></td>
   		<td><?php echo $row[0] ?></td>
   		<td><?php echo $row[1] ?></td>
   		<td><?php echo $row[2] ?></td>
   		<td><?php echo $row[3] ?></td>
  		</tr>
		<?php
		$i++;
		}
	}
		?>
		</tbody>
	</table>
	</body>
</html>
