<template>
  <v-container class="px-8">
    <Modal>
      <v-card-title>
        <span class="headline">Edit Color</span>
      </v-card-title>
      <v-card-text>
        <p>Edit Warna menggunakan kode Hex</p>
        <v-text-field
          label="Kode Hex"
          name="hex"
          v-model="hex"
          required
        ></v-text-field>
      </v-card-text>
      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn
          color="blue darken-1"
          text
          @click="$store.dispatch('setModalColor')"
        >
          Cancel
        </v-btn>
        <v-btn color="blue darken-1" text @click="setColorWithHex()">
          Apply
        </v-btn>
      </v-card-actions>
    </Modal>
    <div class="row-color">
      <div class="left">
        <div class="color-picker">
          <canvas
            id="colors"
            class="pa-2 ml-n1 mt-n1"
            @mousemove="echoColor($event)"
          ></canvas>
          <div id="inner-picker" class="ml-n1 mt-n1"></div>
          <div id="color-slider">
            <div class="mt-2 rgb">
              <label for="red">Red</label>
              <input
                type="range"
                name="red"
                v-model.number="color.red"
                @input="changeColor($event, 'red')"
                id="red"
                min="0"
                max="255"
                value="255"
              />
            </div>
            <div class="mt-2 rgb">
              <label for="green">Green</label>
              <input
                type="range"
                name="green"
                v-model.number="color.green"
                @input="changeColor($event, 'green')"
                id="green"
                min="0"
                max="255"
                value="0"
              />
            </div>
            <div class="mt-2 rgb">
              <label for="blue">Blue</label>
              <input
                type="range"
                name="blue"
                v-model.number="color.blue"
                @input="changeColor($event, 'blue')"
                id="blue"
                min="0"
                max="255"
                value="0"
              />
            </div>
          </div>
        </div>
      </div>
      <div class="right text-center">
        <input
          type="range"
          name="bright"
          v-model.number="color.bright"
          @input="changeColor($event, 'bright')"
          id="bright"
          min="0"
          max="100"
          value="255"
          class="slider ml-1"
        />
        <svg
          fill="white"
          width="24"
          height="24"
          xmlns="http://www.w3.org/2000/svg"
          fill-rule="evenodd"
          clip-rule="evenodd"
          class="ml-1 mt-1"
        >
          <path
            d="M13 24h-2c-.288 0-.563-.125-.753-.341l-.576-.659h4.658l-.576.659c-.19.216-.465.341-.753.341zm1.867-3c.287 0 .52.224.52.5s-.233.5-.52.5h-5.734c-.287 0-.52-.224-.52-.5s.233-.5.52-.5h5.734zm-2.871-17c2.983 0 6.004 1.97 6.004 5.734 0 1.937-.97 3.622-1.907 5.252-.907 1.574-1.843 3.201-1.844 5.014h1.001c0-3.286 3.75-6.103 3.75-10.266 0-4.34-3.502-6.734-7.004-6.734-3.498 0-6.996 2.391-6.996 6.734 0 4.163 3.75 6.98 3.75 10.266h.999c.001-1.813-.936-3.44-1.841-5.014-.938-1.63-1.908-3.315-1.908-5.252 0-3.764 3.017-5.734 5.996-5.734zm9.428 7.958c.251.114.362.411.248.662-.114.251-.41.363-.662.249l-.91-.414c-.252-.114-.363-.41-.249-.662.114-.251.411-.362.662-.248l.911.413zm-18.848 0c-.251.114-.362.411-.248.662.114.251.41.363.662.249l.91-.414c.252-.114.363-.41.249-.662-.114-.251-.411-.362-.662-.248l-.911.413zm18.924-2.958h-1c-.276 0-.5-.224-.5-.5s.224-.5.5-.5h1c.276 0 .5.224.5.5s-.224.5-.5.5zm-18-1c.276 0 .5.224.5.5s-.224.5-.5.5h-1c-.276 0-.5-.224-.5-.5s.224-.5.5-.5h1zm16.818-3.089c.227-.158.284-.469.126-.696-.157-.227-.469-.283-.696-.126l-.821.57c-.227.158-.283.469-.126.696.157.227.469.283.696.126l.821-.57zm-16.636 0c-.227-.158-.284-.469-.126-.696.157-.227.469-.283.696-.126l.821.57c.227.158.283.469.126.696-.157.227-.469.283-.696.126l-.821-.57zm13.333-3.033c.134-.241.048-.546-.193-.68-.241-.135-.546-.048-.68.192l-.488.873c-.135.241-.048.546.192.681.241.134.546.048.681-.193l.488-.873zm-10.03 0c-.134-.241-.048-.546.193-.68.241-.135.546-.048.68.192l.488.873c.135.241.048.546-.192.681-.241.134-.546.048-.681-.193l-.488-.873zm5.515-1.378c0-.276-.224-.5-.5-.5s-.5.224-.5.5v1c0 .276.224.5.5.5s.5-.224.5-.5v-1z"
          />
        </svg>
      </div>
    </div>
    <br />

    <!-- <div class="flex mt-3">
      <button type="button" class="btn manual" onclick="manualOption()">
        Manual
      </button>
      <button type="submit" class="btn apply" onclick="submitColor()">
        Apply
      </button>
    </div> -->

    <!-- <img alt="Vue logo" src="../assets/logo.png" /> -->
    <!-- <HelloWorld msg="Welcome to Your Vue.js App" /> -->
  </v-container>
</template>

<script>
import Modal from "@/components/Modal";
export default {
  name: "Home",
  components: {
    Modal
  },
  data: () => ({
    // color: {
    //   red: 255,
    //   green: 255,
    //   blue: 255,
    //   bright: 100
    // },
    hex: ""
  }),
  mounted() {
    let canvas = document.getElementById("colors");
    let graphics = canvas.getContext("2d");
    canvas.height = canvas.offsetHeight - 10;
    canvas.width = canvas.offsetWidth - 10;
    var CX = canvas.width / 2,
      CY = canvas.height / 2,
      sx = CX,
      sy = CY;
    for (var i = 0; i < 360; i += 0.1) {
      var rad = (i * (2 * Math.PI)) / 360;
      graphics.strokeStyle = "hsla(" + i + ", 100%, 50%, 1.0)";
      graphics.beginPath();
      graphics.moveTo(CX, CY);
      graphics.lineTo(CX + sx * Math.cos(rad), CY + sy * Math.sin(rad));
      graphics.stroke();
    }

    let val = this.color;
    document.getElementById("inner-picker").style.backgroundColor = `rgba(${
      val.red
    }, ${val.green}, ${val.blue}, ${(val.bright * 1) / 100})`;
  },
  computed: {
    color: {
      set(val) {
        this.$store.commit("setColor", val);
      },
      get() {
        return this.$store.state.color;
      }
    }
  },
  watch: {
    color(val) {
      document.getElementById("inner-picker").style.backgroundColor = `rgba(${
        val.red
      }, ${val.green}, ${val.blue}, ${(val.bright * 1) / 100})`;
    }
  },
  methods: {
    setColorWithHex() {
      this.hexToRgb(this.hex);
      this.$store.dispatch("setModalColor");
    },
    hexToRgb(hex) {
      var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);

      this.color = {
        red: parseInt(result[1], 16),
        green: parseInt(result[2], 16),
        blue: parseInt(result[3], 16),
        bright: this.color.bright
      };
      this.setColor({
        red: parseInt(result[1], 16),
        green: parseInt(result[2], 16),
        blue: parseInt(result[3], 16),
        bright: this.color.bright
      });
    },
    echoColor(e) {
      let canvas = document.getElementById("colors");
      let graphics = canvas.getContext("2d");
      var imgData = graphics.getImageData(e.layerX, e.layerY, 2, 2);
      let r = imgData.data[0];
      let g = imgData.data[1];
      let b = imgData.data[2];

      if (r != 0 || g != 0 || b != 0) {
        this.color.red = r;
        this.color.green = g;
        this.color.blue = b;
        this.setColor({ red: r, green: g, blue: b, bright: this.color.bright });
      }
    },
    setColor(value) {
      function convertObjToInt(obj) {
        const res = {};
        for (const key in obj) {
          res[key] = parseInt(obj[key], 10);
        }
        return res;
      }

      this.$store.dispatch("setColor", convertObjToInt(value));
    },
    changeColor(e, selector) {
      this.setColor({
        red: selector == "red" ? e.target.value : this.color.red,
        green: selector == "green" ? e.target.value : this.color.green,
        blue: selector == "blue" ? e.target.value : this.color.blue,
        bright: selector == "bright" ? e.target.value : this.color.bright
      });
    }
  }
};
</script>

<style scoped>
.row-color {
  display: flex;
  width: 85vw;
  height: calc(85vw * 85 / 100);
}
.row-color .left {
  width: 85%; /* height: 100%; */
}
.row-color .right {
  width: 15%;
}
.color-picker {
  position: relative;
  width: 100%;
  height: 100%;
}
.color-picker canvas {
  width: 100%;
  height: 100%;
}
.color-picker::before {
  content: "";
  width: 55%;
  border-radius: 100%;
  height: 55%;
  background-color: var(--bg);
  position: absolute;
  transform: translate(38%, 38%);
}
#inner-picker {
  position: absolute;
  left: 50%;
  top: 50%;
  display: inline-block;
  width: 55%;
  height: 55%;
  border: 10px var(--bg) solid;
  transform: translate(-50%, -50%);
  background: rgb(255, 255, 255);
  border-radius: 100%;
}
.slider {
  height: 85%;
  width: 75%;
  -webkit-appearance: slider-vertical;
}
#red {
  background-image: linear-gradient(90deg, black, #ff0000);
}
#green {
  background-image: linear-gradient(90deg, black, #00ff00);
}
#blue {
  background-image: linear-gradient(90deg, black, #0000ff);
}
.rgb input[type="range"] {
  -webkit-appearance: none;
  margin: 10px 0;
  width: 100%;
  border-radius: 2px;
}
input[type="range"]:disabled {
  background: var(--disabled) !important;
}
.rgb input[type="range"]:focus {
  outline: none;
}
.rgb input[type="range"]::-webkit-slider-runnable-track {
  width: 100%;
  height: 7px;
  cursor: pointer;
}
.rgb input[type="range"]::-webkit-slider-thumb {
  height: 20px;
  width: 20px;
  border-radius: 50%;
  background: #fff;
  box-shadow: 0 0 4px 0 rgba(0, 0, 0, 1);
  cursor: pointer;
  -webkit-appearance: none;
  margin-top: -6px;
}
#color-slider label {
  display: block;
}
#color-slider input {
  width: 100%;
}
#color-preview {
  width: 100%;
  height: 10px;
  background-color: red;
  border-radius: 2px;
}
p {
  margin-bottom: 0;
}
.main-logo {
  height: 120px;
}
</style>
<style>
.main-progress .percent__int {
  font-size: 45px !important;
}
.main-progress .percent__dec,
.main-progress .percent_sign {
  font-size: 22px !important;
}
.main-progress .percent {
  font-size: 35px !important;
}
</style>