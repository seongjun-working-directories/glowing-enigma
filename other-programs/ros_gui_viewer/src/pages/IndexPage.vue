<template>
  <q-page class="row items-center justify-center">
    <q-card-section v-if="!isClicked" class="outer">
      <h4>ROS GUI VIEWER</h4>
      <br/>
      <q-btn color="primary" size="lg" label="LAUNCH ROS GUI Viewer" @click="clicked()" />
    </q-card-section>
    <q-card-section v-else class="outer">
      <h4>ROS GUI VIEWER</h4>
      <br/>
      <q-carousel
        v-model="slide"
        transition-prev="slide-right"
        transition-next="slide-left"
        animated
        control-color="primary"
        style="height: 80%; width: 100%"
        class="rounded-borders">
        <q-carousel-slide name="MapImage" class="column-no-wrap flex-center">
          <img id="MapImage" :src="imgSrc1" alt="" />
        </q-carousel-slide>
        <q-carousel-slide name="MapImage2" class="column-no-wrap flex-center">
          <img id="MapImage2" :src="imgSrc2" alt="" />
        </q-carousel-slide>
        <q-carousel-slide name="MapImage3" class="column-no-wrap flex-center">
          <img style="height:70%" id="MapImage3" :src="imgSrc3" alt="" />
        </q-carousel-slide>
      </q-carousel>
      <div class="row justify-center">
        <q-btn-toggle
          glossy
          v-model="slide"
          :options="[
            { label: 1, value: 'MapImage' },
            { label: 2, value: 'MapImage2' },
            { label: 3, value: 'MapImage3' }
          ]"
        />
      </div>
      <!-- <div>
        <div class="map" style="float:left;">
          <img id="MapImage" :src="imgSrc1" alt="" />
          <br/>
          <img id="MapImage3" :src="imgSrc3" alt="" />
        </div>
        <div style="float:left;">
          <img id="MapImage2" :src="imgSrc2" alt="" />
        </div>
      </div> -->
    </q-card-section>
    <div id="statusIndicator" style="margin:20px;">
      <p id="connecting" :style="connecting">Connecting to rosbridge...</p>
      <p id="connected" :style="connected">Connected</p>
      <p id="error" :style="error">Error!</p>
      <p id="closed" :style="closed">Connection closed...</p>
    </div>
  </q-page>
</template>

<script setup>
/* eslint-disable */
import { ref } from 'vue';
import ROSLIB from 'roslib';

let isClicked = ref(false);
let slide = ref('MapImage');

const ros = new ROSLIB.Ros();

let connecting = ref('display: inline');
let connected = ref('display: none');
let closed = ref('display: none');
let error = ref('display: none');

let imgSrc1 = ref('');
let imgSrc2 = ref('');
let imgSrc3 = ref('');

  // If there is an error on the backend, an 'error' emit will be emitted.
  ros.on('error', (err) => {
    connecting.value = 'display: none';
    connected.value = 'display: none';
    closed.value = 'display: none';
    error.value = 'display: inline';

    console.log(err);
  });

  // Find out exactly when we made a connection.
  ros.on('connection', () => {
    connecting.value = 'display: none';
    connected.value = 'display: inline';
    closed.value = 'display: none';
    error.value = 'display: none';

    console.log('Connected Successfully!');
  });

  ros.on('close', () => {
    connecting.value = 'display: none';
    connected.value = 'display: none';
    closed.value = 'display: inline';
    error.value = 'display: none';

    console.log('Connection closed!');
  });

const mapimg = new ROSLIB.Topic({
  ros: ros,
  name: '/cona/LocalPathPlanner/local_map',
  messageType: 'sensor_msgs/CompressedImage'
});

mapimg.subscribe(m => {
  imgSrc1.value = 'data:image/jpg;base64,' + m.data
});

const mapimg2 = new ROSLIB.Topic({
  ros: ros,
  name: '/cona/GPP_CostalTreeplanning/path_img2',
  messageType: 'sensor_msgs/CompressedImage'
});

mapimg2.subscribe(m => {
  imgSrc2.value = 'data:image/jpg;base64,' + m.data
});

const mapimg3 = new ROSLIB.Topic({
  ros: ros,
  name: '/cona/Sensor/Laser3',
  messageType: 'sensor_msgs/CompressedImage'
});

mapimg3.subscribe(m => {
  imgSrc3.value = 'data:image/jpg;base64,' + m.data
});

function clicked () {
  isClicked.value = !(isClicked.value);


  // Create a connection to the rosbridge WebSocket server.
  ros.connect('ws://192.168.2.2:9090');
}
</script>

<style scoped>
q-carousel-slide {
  width: 1000px;
  height: 1000px;
}
.outer {
  padding:50px;
  margin:50px;
  height:100%;
  width:100%;
  text-align: center;
}

/* #MapImage, #MapImage3 {
  border: 2px solid black;
  height: 300px;
  width: 300px;
}

#MapImage2 {
  border: 2px solid black;
  height: 610px;
  width: 300px;
} */

#connected {
  color: green;
}

#error {
  color: red;
}
</style>
