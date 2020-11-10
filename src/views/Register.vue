<template>
  <div>
    <!-- <div class="wave"></div>
    <div class="wave -two"></div>
    <div class="wave -three"></div> -->
    <div class="text-center px-10">
      <img alt="Jintarkop" class="main-logo mb-3" src="../assets/logo.png" />

      <v-alert class="error" v-if="alert">
        <small>{{ error }}</small>
      </v-alert>
      <p v-else>
        Silahkan mendaftar terlebih dahulu untuk melanjutkan
      </p>
      <v-form ref="form" @submit.prevent="signUp" lazy-validation>
        <v-text-field
          v-model="username"
          :rules="isFilled"
          name="username"
          color="#3076BD"
          label="Nama"
          required
        ></v-text-field>

        <v-text-field
          v-model="email"
          :rules="emailRules"
          name="email"
          color="#3076BD"
          label="Email"
          required
        ></v-text-field>

        <v-text-field
          v-model="password"
          :rules="passwordRules"
          type="password"
          name="password"
          color="#3076BD"
          label="Password"
          required
        ></v-text-field>

        <v-text-field
          v-model="repeat_password"
          :rules="[rePasswordRules]"
          type="password"
          name="repeat_password"
          color="#3076BD"
          label="Ulangi Password"
          required
        ></v-text-field>

        <v-text-field
          v-model="lamp_id"
          :rules="isFilled"
          :counter="10"
          type="text"
          name="lamp_id"
          color="#3076BD"
          label="Kode Seri pada Lampu"
          required
        ></v-text-field>

        <v-dialog v-model="loading" persistent width="300">
          <v-card color="#3076BD" dark>
            <v-card-text>
              Tolong tunggu sebentar...
              <v-progress-linear
                indeterminate
                color="white"
                class="mb-0"
              ></v-progress-linear>
            </v-card-text>
          </v-card>
        </v-dialog>
        <v-btn block dark large color="#EA6A00" class="mt-5 mb-3" type="submit"
          >Register</v-btn
        >
        <span class="text-caption">Sudah punya akun?</span>
        <v-btn block dark large color="#3076BD" class="mt-3" to="/login"
          >Login</v-btn
        >
      </v-form>
    </div>
  </div>
</template>

<script>
export default {
  data: () => ({
    username: "",
    email: "",
    repeat_password: "",
    password: "",
    lamp_id: "",
    alert: false,
    dialog: false
  }),

  computed: {
    rePasswordRules() {
      // (v) => !!v || "Nama belum diisi",
      return () =>
        this.password === this.repeat_password || "Password tidak sama";
      // (v) => (v || "").indexOf(" ") < 0 || "Tidak boleh ada spasi",
    },
    error() {
      return this.$store.state.error;
    },
    loading() {
      return this.$store.state.loading;
    }
  },

  watch: {
    error(value) {
      if (value) {
        this.alert = true;
      }
    },
    alert(value) {
      if (!value) {
        this.$store.commit("setError", null);
      }
    }
  },

  methods: {
    signUp() {
      if (this.$refs.form.validate()) {
        this.dialog = true;
        this.$store.dispatch("userSignUp", {
          username: this.username,
          email: this.email,
          password: this.password,
          lamp_id: this.lamp_id
        });
      }
    }
  }
};
</script>

<style scoped>
.main-logo {
  height: 150px;
}
.absolute {
  position: absolute;
  bottom: 2.5%;
}
@keyframes drift {
  from {
    transform: rotate(0deg);
  }
  from {
    transform: rotate(360deg);
  }
}
</style>
