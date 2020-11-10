<template>
  <v-container class="px-8 mt-5">
    <template v-if="!this.$store.state.user">
      <div class="relative">
        <div class="vertical-center">
          <img alt="Logo" class="main-logo" src="../assets/empty-data.svg" />
          <h3>Oops! Data kosong...</h3>
        </div>
      </div>
    </template>
    <v-form ref="form" @submit.prevent="submit" lazy-validation>
      <v-text-field
        v-model="user.username"
        name="username"
        type="text"
        color="#3076BD"
        label="Username"
      ></v-text-field>
      <v-text-field
        v-model="user.lamp_id"
        name="lamp_id"
        type="text"
        color="#3076BD"
        label="Kode Seri pada Lampu"
        required
      ></v-text-field>

      <v-btn block dark large color="#3076BD" class="mt-5" type="submit"
        >Selesai</v-btn
      >
    </v-form>
    <v-dialog v-model="dialog" persistent max-width="290">
      <v-card>
        <v-card-title class>Apakah data tersebut sudah benar?</v-card-title>
        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="orange darken-1" text @click="dialog = false"
            >Perbaiki</v-btn
          >
          <v-btn color="blue darken-1" text @click="agreeSubmit"
            >Lanjutkan</v-btn
          >
        </v-card-actions>
      </v-card>
    </v-dialog>
  </v-container>
</template>

<script>
export default {
  data: () => ({
    user: {
      username: "",
      lamp_id: ""
    },
    dialog: false
  }),
  mounted() {
    this.user = this.$store.state.user;
  },
  computed: {},
  watch: {
    "$store.state.user": function(e) {
      this.user = e;
    }
  },
  methods: {
    submit() {
      if (this.$refs.form.validate()) {
        this.dialog = true;
      }
    },
    agreeSubmit() {
      this.$store.dispatch("updateUser", this.user);
    }
  }
};
</script>

<style scoped>
.main-logo {
  height: 290px;
}
.relative {
  height: 80vh;
  position: relative;
  text-align: center;
}
</style>

<style>
.v-card__title {
  word-break: unset !important;
}
</style>
