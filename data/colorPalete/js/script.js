const colorPicker = document.getElementById('colorPicker');
const colorDisplay = document.getElementById('colorDisplay');
const switchLed = document.getElementById('switchLed');
const rangeBrightnes = document.getElementById('brightnes')
colorDisplay.style.backgroundColor = colorPicker.value;

colorPicker.addEventListener('input', async function() {
    colorDisplay.style.backgroundColor = colorPicker.value;
    await post_state(switchLed.checked, colorPicker.value, brightnes.value);
});

switchLed.addEventListener('input', async function() {
    await post_state(switchLed.checked, colorPicker.value, brightnes.value);
})

rangeBrightnes.addEventListener('input', async function() {
  await post_state(switchLed.checked, colorPicker.value, brightnes.value);
})

async function post_color(color) {
    try {
        url="http://192.168.31.200/get_color?color=" + color.substring(1, color.length);
        console.log(url);
        await fetch(url, {
            method: 'GET',
            headers: {
              'Accept': 'application/json',
              'Content-Type': 'application/json'
            },
          });
    } catch (e) {
        console.log(e);
    }
}

async function post_state(state, color, brightnes) {
  try {
      body = {
        state: state,
        color: color,
        brightnes: brightnes
      }
      url="http://192.168.31.200/update_state";
      console.log(url);
      console.log(body);
      await fetch(url, {
          method: 'POST',
          headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
          },
          body: JSON.stringify(body)
        });
  } catch (e) {
      console.log(e);
  }
}

document.addEventListener("DOMContentLoaded", function(){
    console.log("page was loaded");
    fetch('/getCurentState')
      .then(response => response.json())
      .then(data => {
        document.getElementById('colorPicker').value = data.color;
        document.getElementById('brightnes').value = data.brightnes;
        document.getElementById('switchLed').checked = data.state
        document.getElementById('colorDisplay').style.backgroundColor = data.color;
      })
      .catch(error => console.error('Error fetching data:', error));
    });