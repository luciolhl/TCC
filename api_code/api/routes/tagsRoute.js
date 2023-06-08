const { Router } = require('express')
const TagController = require('../controllers/TagController')

const router = Router()

// router.get('/tags', (req,res) => {
//   return res.status(200).json({"message": "Api rodando corretamente!"})
// })
router.get('/tags', TagController.pegaTodasAsTags)
// router.get('/tags/ativas', TagController.pegaTodasAsPessoasAtivas)
router.get('/tags/:id', TagController.pegaUmaTag)
router.post('/tags', TagController.criaTag)
router.put('/tags/:id', TagController.atualizarTag)
router.delete('/tags/:id', TagController.apagaTag)
// router.get(
//   '/tags/:estudanteId/matricula',
//   TagController.pegaMatriculaConfirmada
// )
// router.get(
//   '/tags/:estudanteId/matricula/:matriculaId',
//   TagController.pegaUmaMatricula
// )
// router.get(
//   '/tags/matricula/:turmaId/confirmadas',
//   TagController.pegaMatriculasPorTurma
// )
// router.get('/tags/matricula/lotada', TagController.pegaTurmasLotadas)
// router.post('/tags/:estudanteId/cancela', TagController.cancelaPessoa)
// router.get(
//   '/tags/:estudanteId/restaura',
//   TagController.restauraMatriculaPessoa
// )
// router.post('/tags/:estudanteId/matricula', TagController.criaMatricula)
router.post('/tags/:id/restaura', TagController.restauraPessoa)
// router.put(
//   '/tags/:estudanteId/matricula/:matriculaId',
//   TagController.atualizarMatricula
// )
// router.delete(
//   '/tags/:estudanteId/matricula/:matriculaId',
//   TagController.apagaMatricula
// )

module.exports = router
