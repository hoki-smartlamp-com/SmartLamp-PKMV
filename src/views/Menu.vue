<template>
  <v-container>
    <v-card class="pt-3 px-5 mx-4 mt-4 mb-2" height="110">
      <v-row class="">
        <v-col cols="9">
          <h3>{{ user.username }}</h3>
          <p>{{ user.email }}</p>
        </v-col>
        <v-col cols="3" class="text-right">
          <small>{{ user.lamp_id }}</small>
        </v-col>
      </v-row>
      <!-- <v-icon x-large>mdi-cog</v-icon>
      <p class="mt-2">Atur Alat</p> -->
    </v-card>
    <v-row class="text-center mx-1">
      <v-col cols="6">
        <v-card class="pt-10 px-3" height="160" to="/setting">
          <v-icon x-large>mdi-account</v-icon>
          <p class="mt-2">Atur Akun</p>
        </v-card>
      </v-col>
      <v-col cols="6">
        <v-card class="pt-10 px-3" height="160" to="/about">
          <v-icon x-large>mdi-information</v-icon>
          <p class="mt-2">Tentang Kami</p>
        </v-card>
      </v-col>
      <v-col cols="6">
        <v-card class="pt-10 px-3" height="160" to="/feedback">
          <v-icon x-large>mdi-message-draw</v-icon>
          <p>Beri Masukan</p>
        </v-card>
      </v-col>
      <v-col cols="6">
        <v-card class="pt-10 px-3" height="160" @click="signOut">
          <v-icon x-large>mdi-logout</v-icon>
          <p class="mt-2">Keluarkan akun</p>
        </v-card>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
export default {
  data: () => ({}),
  computed: {
    user() {
      return this.$store.state.user;
    }
  }
};
</script>