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
        Silahkan Login terlebih dahulu untuk melanjutkan
      </p>
      <v-form ref="form" @submit.prevent="signIn" lazy-validation>
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
        <v-btn block dark large color="#3076BD" class="mt-5 mb-3" type="submit"
          >Login</v-btn
        >
        <span class="text-caption">Atau</span>
        <v-btn block dark large color="#EA6A00" class="mt-3" to="/register"
          >Register</v-btn
        >
      </v-form>
    </div>
  </div>
</template>

<script>
export default {
  data: () => ({
    email: "",
    password: "",
    alert: false
  }),
  methods: {
    async signIn() {
      if (this.$refs.form.validate()) {
        await this.$store.dispatch("userSignIn", {
          email: this.email,
          password: this.password
        });

        if (this.error == "") {
          await this.$router.replace({ name: "Home" });
        }
      }
    }
  },
  computed: {
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
  }
};
</script>

<style scoped>
.main-logo {
  height: 170px;
}
.absolute {
  position: absolute;
  bottom: 2.5%;
}
.wave {
  opacity: 0.9;
  background: #f3a215;
  width: 700px;
  height: 700px;
  margin-left: -150px;
  margin-top: -170px;
  transform-origin: 50% 48%;
  border-radius: 43%;
  animation: drift-data-v-26084dc2 10000ms infinite linear;
}

.wave.-three {
  top: 5%;
  animation: drift 8500ms infinite linear;
  opacity: 0.5;
  position: absolute;
}

.wave.-two {
  top: 2%;
  animation: drift 7000ms infinite linear;
  position: absolute;
  opacity: 0.7;
  background: #fcad1ba4;
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
