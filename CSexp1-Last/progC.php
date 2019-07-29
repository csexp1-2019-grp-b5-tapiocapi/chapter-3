<!DOCTYPE html>
<html lang="ja">
	<head>
		<title>実装Ｃの結果</title>
		<script type="text/javascript" src="http://code.jquery.com/jquery-1.9.0.js"></script>
		<style type="text/css">
		<!--
		#puzzle{
			display: none;  // 非表示
		}
		#back-curtain{
			background: rgba(0, 0, 0, 0.5); // ③透過率５０％
			display: none;
			position: absolute;
			left: 0;
			top: 0;
		}
		-->
		</style>
		<script type="text/javascript" src="15.js"></script>
	</head>
	<body bgcolor=aqua>
<?php

require 'geotag.php';

if(isset($_REQUEST["tag"])){
	$tag = $_REQUEST["tag"];
	$servername = "localhost";
	$username = "shizutaro";
	$password = "password";
	$dbname = "CSexp1DB";
	$db = new geotag($servername, $username, $password, $dbname);
	$result = $db->search_address_by_postalC($tag);
?>
	<table border="1" width="1000" cellspacing="0" cellpadding="5">
 		<thead>
  		<th bgcolor=#EE0000 scope="col">image</th>
  		<th bgcolor=#EE0000 scope="col">time</th>
  		<th bgcolor=#EE0000 scope="col">latitude</th>
  		<th bgcolor=#EE0000 scope="col">longitude</th>
		<th bgcolor=#EE0000 scope="col">url</th>
		<th bgcolor=#EE0000 scope="col">pazzle</th>
 		</thead>
		<div id="back-curtain"></div>
		<center><canvas id="puzzle" width=640 height=640></canvas></center>
 		<tbody>
		<?php
		$i = 0;
		foreach( $result as $row ) {
		if($i >= 100)break;
		?>
  		<tr>
		<td><img src=<?php echo $row[3] ?> width=150></td>
		<td><?php echo $row[0] ?></td>
   		<td><?php echo $row[1] ?></td>
 		<td><?php echo $row[2] ?></td>
   		<td><?php echo $row[3] ?></td>
		<td><input type="button" value="puzzle" onclick="shuffle('<?php echo $row[3]; ?>')"></td>
		<!--td><iframe src="http://maps.google.co.jp/maps?q=<?php echo $row[1]; ?>,<?php echo $row[2]; ?>&output=embed&t=m&z=16&hl=ja" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" width="200" height="150"></iframe></td!-->
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
