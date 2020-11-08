<template>
  <div class="loading">
    <div class="loader"></div>
  </div>
</template>

<script>
export default {};
</script>

<style>
.loading {
  opacity: 1;
  position: fixed;
  z-index: 10;
  visibility: visible;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  background-color: rgba(0, 0, 0, 0.5);
  backdrop-filter: blur(3px);
  display: flex;
  transition: 0.35s all;
}
.loader {
  margin: auto;
  border: 5px solid #ffffffa6;
  border-radius: 50%;
  width: 10vw;
  height: 10vw;
  -webkit-animation: spin 1.5s linear infinite; /* Safari */
  animation: spin 1.5s linear infinite;
  border-top: 5px solid #ffffff;
  border-bottom: 5px solid #ffffff;
}
@keyframes spin {
  0% {
    transform: rotate(0deg);
  }
  100% {
    transform: rotate(360deg);
  }
}
</style>