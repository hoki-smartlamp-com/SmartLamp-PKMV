<template>
  <v-container class="text-center relative">
    <div class="vertical-center">
      <img alt="Jintarkop" class="main-logo" src="../assets/logo.png" />
      <h2>Maaf anda sedang offline</h2>
      <p>Silahkan hidupkan data atau gunakan wifi lalu refresh kembali</p>

      <p class="my-2">Atau tarik halaman kebawah</p>

      <!-- <v-btn block outlined color="brown" class="mt-3" @click="$router.go()">
        <v-icon left>mdi-refresh</v-icon>Refresh
      </v-btn> -->
      <v-btn block dark large color="#EA6A00" class="mt-4" @click="$router.go()"
        ><v-icon left>mdi-refresh</v-icon>Refresh</v-btn
      >
    </div>
  </v-container>
</template>

<script>
export default {};
</script>

<style scoped>
.main-logo {
  height: 180px;
}
.relative {
  height: 95vh;
  position: relative;
  text-align: center;
}
.vertical-center {
  margin: 0;
  position: absolute;
  top: 50%;
  -ms-transform: translateY(-50%);
  transform: translateY(-50%) translateX(-50%);
  left: 50%;
  width: 50vh;
}
</style>