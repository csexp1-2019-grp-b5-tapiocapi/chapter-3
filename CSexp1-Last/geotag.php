<?php

class geotag
{
	public $pdo;

	public function __construct($servername, $username, $password, $dbname) {
		$dsn = "mysql:dbname=" . $dbname . ";host=" . $servername;
		$this->pdo = null;

		try {
			$this->pdo = new PDO($dsn, $username, $password);
		} catch (PDOException $e) {
			print('Connection failed: ' . $e->getMessage());
			die();
		}
	}

	public function __destruct() {
		$this->pdo = null;
	}

	function search_address_by_postalB($tag) {
		$keyword = "%" . $tag . "%";
		$sql  = "SELECT time,latitude,longitude,url FROM geotag,tag IGNORE INDEX(i_tag) WHERE tag.tag LIKE :tag AND geotag.id = tag.id ORDER by time";
		$stmt = $this->pdo->prepare($sql);
		$stmt->bindParam(':tag', $keyword);
		$stmt->execute();
		return $stmt->fetchAll();
	}

	function search_address_by_postalC($tag) {
		$keyword = "%" . $tag . "%";
		$sql  = "SELECT time,latitude,longitude,url FROM geotag,tag WHERE tag.tag LIKE :tag AND geotag.id = tag.id ORDER by time";
		$stmt = $this->pdo->prepare($sql);
		$stmt->bindParam(':tag', $keyword);
		$stmt->execute();
		return $stmt->fetchAll();
	}
}
?>
