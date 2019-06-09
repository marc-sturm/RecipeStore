<?php

$dir = dirname(__FILE__)."/";
include("{$dir}functions.php");

function add_table(&$output, $nodes, $name = "")
{
	if ($name!="")
	{
		xml_open_tag($output, "tr");
		xml_open_tag($output, "td", array("colspan"=>"3"));
		xml_open_tag($output, "b", array(), true, "$name");
		xml_close_tag($output); //td
		xml_close_tag($output); //tr
	}
	
	$row_open = false;
	foreach($nodes as $node)
	{
		if ($node->nodeType == XML_ELEMENT_NODE && $node->nodeName=="ingr")
		{
			if($row_open)
			{
				xml_close_tag($output);
			}
			
			xml_open_tag($output, "tr");
			$row_open = true;
			
			xml_open_tag($output, "td", array("valign"=>"top"), true, "<nobr>".$node->getAttribute("amount")." ".$node->getAttribute("unit")."</nobr>");
			xml_open_tag($output, "td", array("valign"=>"top"), true, "<nobr>".$node->getAttribute("name")."</nobr>");
		}
		else if ($node->nodeType == XML_TEXT_NODE)
		{
			$text = trim($node->nodeValue);
			if ($text=="") continue;
			
			if(!$row_open)
			{
				xml_open_tag($output, "tr");
				xml_open_tag($output, "td", array(), true);
				xml_open_tag($output, "td", array(), true);
				
				$row_open = true;
			}
			
			xml_open_tag($output, "td", array("valign"=>"top"), true, $text);
		}
		else
		{
			print "Warning: Unknown DOM node type!";
		}
	}
	xml_close_tag($output); //tr
}

// Check input file
$file = file_get_contents("{$dir}Kochbuch.xml");

$messages = array();
if (!xml_is_wellformed($file, $messages))
{
	print "\n";
	print "Error: XML output is NOT wellfomed:\n";
	xml_print_messages($messages);
	exit();
}

if (!xml_matches_schema($file, file_get_contents("{$dir}Kochbuch.xsd"), $messages))
{
	print "\n";
	print "Error: XML output does not match the schema:\n";
	xml_print_messages($messages);
	exit();
}

//add toggle javascript
$output = array();
$output[] = "<html>";
$output[] = "  <head>";
$output[] = "    <title>Kochbuch</title>";
$output[] = "    <meta charset=\"utf-8\"/>";
$output[] = "    <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" />";
$output[] = "  </head>";
$output[] = "<script language=\"javascript\">";
$output[] = "function toggle(i)";
$output[] = "{";
$output[] = "	var ele = document.getElementById(\"toggle\" + i);";
$output[] = "	var text = document.getElementById(\"displayText\");";
$output[] = "	if(ele.style.display == \"block\") {";
$output[] = "    		ele.style.display = \"none\";";
$output[] = "		text.innerHTML = \"show\";";
$output[] = "  	}";
$output[] = "	else {";
$output[] = "		ele.style.display = \"block\";";
$output[] = "		text.innerHTML = \"hide\";";
$output[] = "	}";
$output[] = "} ";
$output[] = "</script>";


//sort recipies by course
$courses = array("Vorspeise" => array(), "Suppe" => array(), "Salat" => array(), "Hauptgang" => array(), "Beilage" => array(), "Dessert" => array(), "Dip" => array(), "Kuchen" => array(), "Backen" => array(), "Cocktail" => array(), "Alt" => array());
$doc = new DOMDocument();
$doc->load("{$dir}Kochbuch.xml");
$collection = $doc->getElementsByTagName("recipe");
foreach($collection as $recipe)
{
	$name = $recipe->getAttribute("name");
	$course = $recipe->getAttribute("course");
	$courses[$course][$name] = $recipe;
}

//write courses
$i=0;
foreach($courses as $course => $name2recipe)
{
		if ($course=="Alt") continue;
		
		if ($i!=0)
		{
			xml_open_tag($output, "br", array(), true);
			xml_open_tag($output, "br", array(), true);
		}
		xml_open_tag($output, "b", array(), true, "- ".ucfirst($course)." -");
		
		foreach($name2recipe as $name => $recipe)
		{
			xml_open_tag($output, "br", array(), true);
			xml_open_tag($output, "a", array("href"=>"javascript:toggle($i);", "class"=>"dec"), true, $name);
			
			xml_open_tag($output, "div", array("id"=>"toggle$i", "style"=>"display: none"));
			
			$amount = $recipe->getAttribute("amount");
			xml_open_tag($output, "font", array(), true, "(".$amount.")");
			xml_open_tag($output, "br", array(), true);
			xml_open_tag($output, "br", array(), true);
			
			$parts = $recipe->getElementsByTagName("part");
			if ($parts->length == 0)
			{
				xml_open_tag($output, "table");
				add_table($output, $recipe->childNodes);
				xml_close_tag($output); //table
			}
			else
			{	
				xml_open_tag($output, "table");
				foreach($parts as $part)
				{
					add_table($output, $part->childNodes, $part->getAttribute("name"));
				}
				xml_close_tag($output); //table
			}
			
			xml_close_tag($output); //div
			++$i;
		}
		xml_close_tag($output); //p
}

$output[] = "  </body>";
$output[] = "</html>";

file_put_contents("{$dir}/kochbuch.html", implode("\n",$output));

fgets(STDIN);

?>