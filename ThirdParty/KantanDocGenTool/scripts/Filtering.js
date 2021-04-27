function onPageLoaded()
{
	var groups = [ "" ];
	var divs = document.getElementsByClassName("group_type");
	for (var i = 0; i < divs.length; i++)
	{
		var g = divs[i].innerHTML.trim();
		if (-1 == groups.indexOf(g))
			groups.push(g);
	}
	groups.sort();
	groups.splice(0, 0, "everything");
	
	var options = '';
	for (var i = 0; i < groups.length; i++) 
	{
		options += '<option value="' + groups[i] + '">'+ groups[i] +'</option>';
	}
	document.getElementById("groupfilters").innerHTML = options;
}
function onFilterChanged()
{
	var filter = document.getElementById("groupfilters");
	console.log("["+filter.value+"]");
	
	var divs = document.getElementsByClassName("group_type");
	for (var i = 0; i < divs.length; i++)
	{
		const visible = filter.value == "everything" || divs[i].innerHTML.trim() == filter.value;
		divs[i].parentNode.parentNode.style.display = visible ? "table-row" : "none";
	}
}
