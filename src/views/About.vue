<template>
  <v-container class="px-5">
    <div class="text-center mb-2">
      <img alt="Jintarkop" class="main-logo mb-3" src="../assets/logo.png" />
    </div>
    <v-card class="px-4 pt-4 pb-3">
      <p class="mb-3">
        <span class="font-weight-bold blue--text">Hoki SmartLamp</span>
        merupakan inovasi lampu hias berbasis Internet Of Things pertama dengan
        bahan dasar limbah bambu dan kayu yang bermotif unik serta mengedepankan
        kearifan lokal, kontrol hoki smart lamp melalui aplikasi. Ini lebih dari
        sekedar cara yang berbeda untuk mengontrol pencahayaan di rumah anda.
      </p>
      <h4 class="blue--text mt-5">Kontak :</h4>
      <div class="mt-2">
        <v-btn
          block
          dark
          large
          color="red accent-3"
          class="mb-4"
          href="https://www.instagram.com/hoki_smartlamp/"
          ><v-icon>mdi-instagram</v-icon>@hoki_smartlamp</v-btn
        >
        <v-btn
          block
          dark
          large
          color="green darken-2"
          class="mb-4"
          href="https://wa.me/6282141226299"
          ><v-icon>mdi-whatsapp</v-icon>+62 82141226299
        </v-btn>
      </div>
    </v-card>
  </v-container>
</template>

<script >
export default {};
</script>

<style scoped>
p {
  margin-bottom: 0;
}
.main-logo {
  height: 160px;
}
.v-card p {
  text-align: justify;
}
</style>