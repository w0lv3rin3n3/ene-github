const feedTemp = document.querySelector("#feedTemp");
fetch("http://localhost:3000/dht11-sensor")
  .then((response) => response.json())
  .then((data) => {
    const ctx = document.getElementById("myChartTemp").getContext("2d");
    const dateTimeArr = [];
    for(let element of data) {
      // moment(element.date).format('HH:mm')
      dateTimeArr.push(element.date);
    }
    const valuesArr = [];
    for(let element of data) {
      if(element.sensor_name == 'dht11_sensor')
        valuesArr.push(element.value1);
    }
    const myChartTemp = new Chart(ctx, {
      type: "line",
      data: {
        //array of strings (datetimes)
        labels: dateTimeArr,
        datasets: [
          {
            label: "Temperature",
            //array of datetime values
            data: valuesArr, 
            backgroundColor: [
              "rgba(10, 200, 10, 0.2)",
            ],
            borderColor: [
              "rgba(50, 100, 50, 1)",
            ],
            borderWidth: 2,
          },
        ],
      },
      options: {
        responsive: true,
        scales: {
          xAxes: [{
            display: false
          }],
          y: {
            beginAtZero: true,
          },
        },
      },
    });
    feedTemp.insertAdjacentHTML(
      "afterbegin",
      data[data.length-1].value1 + '&#8451'
    );
  })
  .catch((err) => console.log(err));



//   setTimeout(function(){
//     window.location.reload(1);
//  }, 20000);