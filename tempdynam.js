var margin = {top: 100, right: 20, bottom: 30, left: 230},
      width = 400;// - margin.left - margin.right,
      height = 200;// - margin.top - margin.bottom;

  var div = d3.select("body").append("div")	
        .attr("class", "tooltip")				
        .style("opacity", 0);
  
  // parse the date / time
  var parseTime = d3.timeParse("%H:%M:%S");
  
  // set the ranges
  var x = d3.scaleTime().range([0, width]);
  var y = d3.scaleLinear().range([height, 0]);
  
/*  // define the line
  var valueline = d3.line()
      .x(function(d) { return x(d.Date); })
      .y(function(d) { return y(d.hum); });
 */ // define the line
  var valueline2 = d3.line()
      .x(function(d) { return x(d.Date); })
      .y(function(d) { return y(d.temp); });
/*  // define the line
  var valueline3 = d3.line()
      .x(function(d) { return x(d.Date); })
      .y(function(d) { return y(d.pm25); });
   // define the line
  var valueline4 = d3.line()
      .x(function(d) { return x(d.Date); })
      .y(function(d) { return y(d.pm10); });
  
 */   
  // append the svg obgect to the body of the page
  // appends a 'group' element to 'svg'
  // moves the 'group' element to the top left margin
  var svg4 = d3.select("body div#tempdy").append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
    .append("g")
      .attr("transform","translate(" + margin.left + "," + margin.top + ")");
  
  function draw4(data) {
    
    var data = data;
    
    // format the data
    data.forEach(function(d) {
        d.Date = parseTime(d.time);
    //    d.hum = +d.hum;
        d.temp = +d.temp;
     /*   d.pm25 = +d.pm25;
        d.pm10 = +d.pm10;
	*/	
    });
    
    // sort years ascending
    /*data.sort(function(a, b){
      return a["Date"]-b["Date"];
    })*/
   
    // Scale the range of the data
    x.domain(d3.extent(data, function(d) { return d.Date; }));
    y.domain([0, d3.max(data, function(d) {
      return Math.max(/*d.hum, */d.temp/*, d.pm25, d.pm10*/); })]);
	  
		  
	    // text label for the x axis
       svg4.append("text")      
        .attr("x", 210)
        .attr("y",  230)
        .style("text-anchor", "middle")
        .text("Time");
    // text label for the y axis
       svg4.append("text")     
        .attr("y", 100)
        .attr("x",  -45)
        .style("text-anchor", "middle")
        .text("°C");  
    
 /*   // Add the valueline path.
    svg.append("path")
        .data([data])
        .attr("class", "line")
        .attr("d", valueline);
  */  // Add the valueline path.
    svg4.append("path")
        .data([data])
        .attr("class", "line2")
        .attr("d", valueline2);  
 /*   // Add the valueline path.
    svg.append("path")
        .data([data])
        .attr("class", "line3")
        .attr("d", valueline3); 
    svg.append("path")
        .data([data])
        .attr("class", "line4")
        .attr("d", valueline4); 
*/		
    // Add the X Axis
    svg4.append("g")
        .attr("transform", "translate(0," + height + ")")
        .call(d3.axisBottom(x));
  
    // Add the Y Axis
    svg4.append("g")
        .call(d3.axisLeft(y));
  
/*    svg.selectAll("dot").data(data)
    .enter()
    .append("circle")
    .attr("r",5)
    .attr("cx",function(d){return x(d.Date)})
    .attr("cy",function(d){return y(d.hum);})
    .attr("class","dot")
    .on("mouseover", function(d) {		
        div.transition()		
            .duration(200)		
            .style("opacity", 1000);		
        div.html("<b>hum</b>:" +d.hum + "<br/>" +"<b>Time</b>:"+d.time)	
            .style("left", (d3.event.pageX) + "px")		
            .style("top", (d3.event.pageY - 28) + "px");	
    })					
    .on("mouseout", function(d) {		
        div.transition()		
            .duration(1000)		
            .style("opacity", 0);	
    });
  
 */   svg4.selectAll("dot").data(data)
    .enter()
    .append("circle")
    .attr("r",5)
    .attr("cx",function(d){return x(d.Date)})
    .attr("cy",function(d){return y(d.temp);})
    .attr("class","dot1")
    .on("mouseover", function(d) {		
        div.transition()		
            .duration(200)		
            .style("opacity", 1000);		
        div.html("<b>temp</b>:" +d.temp + "<br/>" +"<b>Time</b>:"+d.time)	
            .style("left", (d3.event.pageX) + "px")		
            .style("top", (d3.event.pageY - 28) + "px");	
    })					
    .on("mouseout", function(d) {		
        div.transition()		
            .duration(1000)		
            .style("opacity", 0);	
    });
  
 /*   svg.selectAll("dot").data(data)
    .enter()
    .append("circle")
    .attr("r",5)
    .attr("cx",function(d){return x(d.Date)})
    .attr("cy",function(d){return y(d.pm25);})
    .attr("class","dot3")
    .on("mouseover", function(d) {		
        div.transition()		
            .duration(200)		
            .style("opacity", 1000);		
        div.html("<b>pm25</b>:" +d.pm25 + "<br/>" +"<b>Time</b>:"+d.time)	
            .style("left", (d3.event.pageX) + "px")		
            .style("top", (d3.event.pageY - 28) + "px");	
    })					
    .on("mouseout", function(d) {		
        div.transition()		
            .duration(1000)		
            .style("opacity", 0);	
    });
 svg.selectAll("dot").data(data)
    .enter()
    .append("circle")
    .attr("r",5)
    .attr("cx",function(d){return x(d.Date)})
    .attr("cy",function(d){return y(d.pm10);})
    .attr("class","dot4")
    .on("mouseover", function(d) {		
        div.transition()		
            .duration(200)		
            .style("opacity", 1000);		
        div.html("<b>pm10</b>:" +d.pm10 + "<br/>" +"<b>Time</b>:"+d.time)	
            .style("left", (d3.event.pageX) + "px")		
            .style("top", (d3.event.pageY - 28) + "px");	
    })					
    .on("mouseout", function(d) {		
        div.transition()		
            .duration(1000)		
            .style("opacity", 0);	
    });
*/	
  }
  
  // Get the data
  d3.json("http://23.254.253.63/FlaskApp/air_data", function(error, data) {
    if (error) throw error;
    
    // trigger render
    draw4(data);
    
    
  });
  