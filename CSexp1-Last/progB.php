<!DOCTYPE html>
<html lang="ja"> 
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/pace/1.0.2/themes/green/pace-theme-loading-bar.css" />
    <script src="https://code.jquery.com/jquery-1.11.3.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/pace/1.0.2/pace.min.js"></script>
	<head>
		<title>実装Ｂの結果</title>
	</head>
	<body bgcolor=lime>
<?php

require 'geotag.php';

if(isset($_REQUEST["tag"])){
	$tag = $_REQUEST["tag"];
	$servername = "localhost";
	$username = "shizutaro";
	$password = "password";
	$dbname = "CSexp1DB";
	$db = new geotag($servername, $username, $password, $dbname);
	$result = $db->search_address_by_postalB($tag);
?>
	<table border="1" width="1000" cellspacing="0" cellpadding="5">
 		<thead>
  		<th bgcolor=#EE0000 scope="col">image</th>
  		<th bgcolor=#EE0000 scope="col">time</th>
  		<th bgcolor=#EE0000 scope="col">latitude</th>
  		<th bgcolor=#EE0000 scope="col">longitude</th>
		<th bgcolor=#EE0000 scope="col">url</th>
		<!--th bgcolor=#EE0000 scope="col">map</th!-->
 		</thead>
 		<tbody>
		<?php
		$i = 0;
		foreach( $result as $row ) {
		if($i >= 100)break;
		?>
  		<tr>
		<td><img src=<?php echo $row[3] ?> width=150></td>
	<?php	 
		for($j=0;$j<4;$j++){
	?>
   		<td><?php echo $row[$j] ?></td>
	<?php	
		}
	?>
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
