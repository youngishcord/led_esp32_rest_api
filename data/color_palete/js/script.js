const colorPicker = document.getElementById('colorPicker');
const colorDisplay = document.getElementById('colorDisplay');

colorPicker.addEventListener('input', async function() {
    colorDisplay.style.backgroundColor = colorPicker.value;
    // console.log(colorPicker.value);

    await post_color(colorPicker.value);
});

async function post_color(color) {
    try {
        await fetch("http://192.168.31.200/" + color, {
            method: 'POST',
            headers: {
              'Accept': 'application/json',
              'Content-Type': 'application/json'
            },
          });
    } catch (e) {
        console.log(e);
    }
}

// jscolor {onFineChange:'update(this)'}

// function update(picker) {
//     document.getElementById('colorPicker').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
//     client.println("document.getElementById(\"change_color\").href=\"?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}